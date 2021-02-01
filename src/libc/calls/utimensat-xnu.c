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
#include "libc/calls/internal.h"
#include "libc/nexgen32e/nexgen32e.h"
#include "libc/sysv/consts/at.h"
#include "libc/sysv/consts/utime.h"
#include "libc/sysv/errfuns.h"
#include "libc/time/time.h"

int utimensat$xnu(int dirfd, const char *path, const struct timespec ts[2],
                  int flags) {
  int i;
  struct timeval now, tv[2];
  if (flags) return einval();
  if (!ts || ts[0].tv_nsec == UTIME_NOW || ts[1].tv_nsec == UTIME_NOW) {
    gettimeofday(&now, NULL);
  }
  if (ts) {
    for (i = 0; i < 2; ++i) {
      if (ts[i].tv_nsec == UTIME_NOW) {
        tv[i] = now;
      } else if (ts[i].tv_nsec == UTIME_OMIT) {
        return einval();
      } else {
        tv[i].tv_sec = ts[i].tv_sec;
        tv[i].tv_usec = div1000int64(ts[i].tv_nsec);
      }
    }
  } else {
    tv[0] = now;
    tv[1] = now;
  }
  if (path) {
    if (dirfd == AT_FDCWD) {
      return utimes$sysv(path, tv);
    } else {
      return enosys();
    }
  } else {
    if (dirfd != AT_FDCWD) {
      return futimes$sysv(dirfd, tv);
    } else {
      return einval();
    }
  }
}
