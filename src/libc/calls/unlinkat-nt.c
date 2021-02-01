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
#include "libc/errno.h"
#include "libc/nt/errors.h"
#include "libc/nt/files.h"
#include "libc/nt/runtime.h"
#include "libc/nt/synchronization.h"
#include "libc/sysv/consts/at.h"

static textwindows int unlink$nt(const char16_t *path) {
  if (DeleteFile(path)) {
    return 0;
  } else {
    return __winerr();
  }
}

static textwindows int rmdir$nt(const char16_t *path) {
  int e, ms;
  for (ms = 1;; ms *= 2) {
    if (RemoveDirectory(path)) return 0;
    /*
     * Files can linger, for absolutely no reason.
     * Possibly some Windows Defender bug on Win7.
     * Sleep for up to one second w/ expo backoff.
     * Alternative is use Microsoft internal APIs.
     * Never could have imagined it'd be this bad.
     */
    if ((e = GetLastError()) == kNtErrorDirNotEmpty && ms <= 512) {
      Sleep(ms);
      continue;
    } else {
      break;
    }
  }
  errno = e;
  return -1;
}

textwindows int unlinkat$nt(int dirfd, const char *path, int flags) {
  uint16_t path16[PATH_MAX];
  if (__mkntpathat(dirfd, path, 0, path16) == -1) return -1;
  if (flags & AT_REMOVEDIR) {
    return rmdir$nt(path16);
  } else {
    return unlink$nt(path16);
  }
}
