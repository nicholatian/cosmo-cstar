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
#include "libc/nexgen32e/nexgen32e.h"
#include "libc/testlib/testlib.h"

void *_memset(void *, int, size_t) asm("memset");

TEST(memset, size0_doesNothing) {
  _memset(NULL, 0, 0);
}

TEST(memset, size1) {
  char *b = tgc(tmalloc(1));
  _memset(b, 7, 1);
  EXPECT_EQ(7, b[0]);
}

TEST(memset, size2) {
  char *b = tgc(tmalloc(2));
  _memset(b, 7, 2);
  EXPECT_EQ(7, b[0]);
  EXPECT_EQ(7, b[1]);
}

TEST(memset, size3) {
  char *b = tgc(tmalloc(3));
  _memset(b, 7, 3);
  EXPECT_EQ(7, b[0]);
  EXPECT_EQ(7, b[1]);
  EXPECT_EQ(7, b[2]);
}

TEST(memset, size4) {
  char *b = tgc(tmalloc(4));
  _memset(b, 7, 4);
  EXPECT_EQ(7, b[0]);
  EXPECT_EQ(7, b[1]);
  EXPECT_EQ(7, b[2]);
  EXPECT_EQ(7, b[3]);
}

TEST(memset, size5) {
  char *b = tgc(tmalloc(5));
  _memset(b, 7, 5);
  EXPECT_EQ(7, b[0]);
  EXPECT_EQ(7, b[1]);
  EXPECT_EQ(7, b[2]);
  EXPECT_EQ(7, b[3]);
  EXPECT_EQ(7, b[4]);
}

TEST(memset, testMulTrick4) {
  long i, j;
  unsigned long x;
  long di, si, dx, ax;
  volatile uint8_t *b;
  b = tgc(tmalloc(4));
  for (i = 0; i < 255; ++i) {
    for (j = -1; j < 1; ++j) {
      x = j;
      x &= ~0xff;
      x |= i;
      asm volatile("call\tmemset"
                   : "=D"(di), "=S"(si), "=d"(dx), "=a"(ax)
                   : "0"(b), "1"(x), "2"(4)
                   : "rcx", "memory", "cc");
      ASSERT_EQ(x & 0xff, b[0]);
      ASSERT_EQ(x & 0xff, b[1]);
      ASSERT_EQ(x & 0xff, b[2]);
      ASSERT_EQ(x & 0xff, b[3]);
    }
  }
}

TEST(memset, testMulTrick8) {
  long i, j;
  unsigned long x;
  long di, si, dx, ax;
  volatile uint8_t *b;
  b = tgc(tmalloc(8));
  for (i = 0; i < 255; ++i) {
    for (j = -1; j < 1; ++j) {
      x = j;
      x &= ~0xff;
      x |= i;
      asm volatile("call\tmemset"
                   : "=D"(di), "=S"(si), "=d"(dx), "=a"(ax)
                   : "0"(b), "1"(x), "2"(8)
                   : "rcx", "memory", "cc");
      ASSERT_EQ(x & 0xff, b[0]);
      ASSERT_EQ(x & 0xff, b[1]);
      ASSERT_EQ(x & 0xff, b[2]);
      ASSERT_EQ(x & 0xff, b[3]);
      ASSERT_EQ(x & 0xff, b[4]);
      ASSERT_EQ(x & 0xff, b[5]);
      ASSERT_EQ(x & 0xff, b[6]);
      ASSERT_EQ(x & 0xff, b[7]);
    }
  }
}
