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
#include "libc/bits/weaken.h"
#include "libc/calls/calls.h"
#include "libc/calls/internal.h"
#include "libc/errno.h"
#include "libc/sysv/consts/at.h"
#include "libc/zipos/zipos.internal.h"

/**
 * Returns information about thing.
 *
 * @param dirfd is normally AT_FDCWD but if it's an open directory and
 *     file is a relative path, then file becomes relative to dirfd
 * @param st is where result is stored
 * @param flags can have AT_{EMPTY_PATH,NO_AUTOMOUNT,SYMLINK_NOFOLLOW}
 * @return 0 on success, or -1 w/ errno
 * @see S_ISDIR(st.st_mode), S_ISREG()
 * @asyncsignalsafe
 */
int fstatat(int dirfd, const char *path, struct stat *st, uint32_t flags) {
  struct ZiposUri zipname;
  if (weaken(__zipos_stat) && weaken(__zipos_parseuri)(path, &zipname) != -1) {
    return weaken(__zipos_stat)(&zipname, st);
  } else if (!IsWindows()) {
    return fstatat$sysv(dirfd, path, st, flags);
  } else {
    return fstatat$nt(dirfd, path, st, flags);
  }
}
