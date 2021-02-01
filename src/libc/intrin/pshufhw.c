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
#include "libc/intrin/pshufhw.h"

/**
 * Shuffles lower half of word vector.
 * @param 𝑚 needs to be a literal, constexpr, or embedding
 * @mayalias
 */
void(pshufhw)(int16_t b[8], const int16_t a[8], uint8_t m) {
  int16_t t[4];
  t[0] = a[4 + ((m & 0b00000011) >> 0)];
  t[1] = a[4 + ((m & 0b00001100) >> 2)];
  t[2] = a[4 + ((m & 0b00110000) >> 4)];
  t[3] = a[4 + ((m & 0b11000000) >> 6)];
  b[0] = a[0];
  b[1] = a[1];
  b[2] = a[2];
  b[3] = a[3];
  b[4] = t[0];
  b[5] = t[1];
  b[6] = t[2];
  b[7] = t[3];
}
