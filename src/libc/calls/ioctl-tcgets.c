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
#include "libc/calls/struct/metatermios.internal.h"
#include "libc/calls/termios.internal.h"
#include "libc/sysv/consts/termios.h"

int ioctl$tcgets$nt(int, struct termios *);

static int ioctl$tcgets$sysv(int fd, struct termios *tio) {
  int rc;
  union metatermios t;
  if ((rc = ioctl$sysv(fd, TCGETS, &t)) != -1) {
    termios2linux(tio, &t);
  }
  return rc;
}

/**
 * Returns information about terminal.
 *
 * @see tcgetattr(fd, tio) dispatches here
 * @see ioctl(fd, TCGETS, tio) dispatches here
 * @see ioctl(fd, TIOCGETA, tio) dispatches here
 */
int ioctl$tcgets(int fd, struct termios *tio) {
  if (!IsWindows()) {
    return ioctl$tcgets$sysv(fd, tio);
  } else {
    return ioctl$tcgets$nt(fd, tio);
  }
}
