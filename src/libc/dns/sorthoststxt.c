/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/alg/alg.h"
#include "libc/dns/dns.h"
#include "libc/dns/hoststxt.h"

/**
 * Compares hostnames in HOSTS.TXT table.
 * @see dnsnamecmp(), parsehoststxt()
 */
static int cmphoststxt(const struct HostsTxtEntry *e1,
                       const struct HostsTxtEntry *e2, const char *strings) {
  if (e1 == e2) return 0;
  return dnsnamecmp(&strings[e1->name], &strings[e2->name]);
}

/**
 * Sorts entries in HOSTS.TXT table.
 *
 * This function enables resolvehoststxt() to be called so hard-coded
 * hostname lookups take logarithmic time; you can blackhole all the
 * spam you want, in your /etc/hosts file.
 *
 * The sorted order, defined by dnsnamecmp(), also makes it possible to
 * efficiently search for subdomains, once the initial sort is done.
 */
void sorthoststxt(struct HostsTxt *ht) {
  if (ht->entries.p) {
    qsort_r(ht->entries.p, ht->entries.i, sizeof(*ht->entries.p),
            (void *)cmphoststxt, ht->strings.p);
  }
}
