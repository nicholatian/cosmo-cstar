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
#include "libc/bits/bits.h"
#include "libc/macros.h"
#include "libc/runtime/runtime.h"
#include "libc/sysv/errfuns.h"

static struct AtFork {
  volatile size_t i;
  struct AtForkCallback {
    void (*fn)(void *);
    void *arg;
  } p[ATEXIT_MAX];
} g_atfork;

/**
 * Registers function to be called by fork() in child.
 *
 * @return 0 on success, or -1 w/ errno
 * @note vfork() won't invoke callbacks
 * @asyncsignalsafe
 */
int atfork(void *fn, void *arg) {
  size_t i;
  for (;;) {
    i = g_atfork.i;
    if (i == ARRAYLEN(g_atfork.p)) return enomem();
    if (cmpxchg(&g_atfork.i, i, i + 1)) {
      g_atfork.p[i] = (struct AtForkCallback){.fn = fn, .arg = arg};
      return 0;
    }
  }
}

/**
 * Triggers callbacks registered by atfork().
 *
 * @note only fork() should call this
 * @asyncsignalsafe
 */
void __onfork(void) {
  size_t i;
  for (i = 0; i < g_atfork.i; ++i) {
    g_atfork.p[i].fn(g_atfork.p[i].arg);
  }
}
