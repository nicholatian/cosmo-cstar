/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2021 Justine Alexandra Roberts Tunney                              │
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
#include "libc/calls/calls.h"
#include "libc/errno.h"
#include "libc/fmt/conv.h"
#include "libc/fmt/fmt.h"
#include "libc/log/check.h"
#include "libc/macros.h"
#include "libc/runtime/gc.h"
#include "libc/testlib/testlib.h"
#include "libc/x/x.h"

char basedir[PATH_MAX];
char testdir[PATH_MAX];

void SetUp(void) {
  getcwd(basedir, ARRAYLEN(basedir));
  sprintf(testdir, "o/tmp/%s.%d", program_invocation_short_name, getpid());
  makedirs(testdir, 0755);
  CHECK_NE(-1, chdir(testdir));
}

void TearDown(void) {
  CHECK_NE(-1, chdir(basedir));
  CHECK_NE(-1, rmrf(testdir));
}

TEST(getcwd, test) {
  char buf[PATH_MAX];
  EXPECT_NE(-1, mkdir("subdir", 0755));
  EXPECT_NE(-1, chdir("subdir"));
  EXPECT_STREQ("subdir", basename(getcwd(buf, ARRAYLEN(buf))));
}

TEST(getcwd, testNullBuf_allocatesResult) {
  EXPECT_NE(-1, mkdir("subdir", 0755));
  EXPECT_NE(-1, chdir("subdir"));
  EXPECT_STREQ("subdir", basename(gc(getcwd(NULL, 0))));
}
