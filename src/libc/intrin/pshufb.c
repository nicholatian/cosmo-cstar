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
#include "libc/intrin/pshufb.h"
#include "libc/str/str.h"

/**
 * Shuffles and/or clears 8-bit integers.
 *
 * @param 𝑎 [w/o] receives result
 * @param 𝑏 [r/o] supplies byte vector
 * @param 𝑐 [r/o] supplies mask vector
 * @note goes fast w/ ssse3 (intel c. 2004, amd c. 2011)
 * @note doesn't perfectly emulate mmx
 * @mayalias
 */
void(pshufb)(uint8_t a[16], const uint8_t b[16], const uint8_t c[16]) {
  unsigned i;
  uint8_t r[16];
  for (i = 0; i < 16; ++i) r[i] = (c[i] & 0x80) ? 0 : b[c[i] & 0x0F];
  memcpy(a, r, 16);
}
