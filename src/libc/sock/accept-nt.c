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
#include "libc/nt/files.h"
#include "libc/nt/struct/pollfd.h"
#include "libc/nt/winsock.h"
#include "libc/sock/internal.h"
#include "libc/sock/yoink.inc"
#include "libc/sysv/consts/fio.h"
#include "libc/sysv/consts/poll.h"
#include "libc/sysv/consts/sock.h"
#include "libc/sysv/errfuns.h"

textwindows int accept$nt(struct Fd *fd, void *addr, uint32_t *addrsize,
                          int flags) {
  int64_t h;
  int client;
  uint32_t yes;
  for (;;) {
    if (!WSAPoll(&(struct pollfd$nt){fd->handle, POLLIN}, 1, 1000)) continue;
    if ((client = __getemptyfd()) == -1) return -1;
    if ((h = WSAAccept(fd->handle, addr, (int32_t *)addrsize, 0, 0)) != -1) {
      if (flags & SOCK_NONBLOCK) {
        yes = 1;
        if (__ioctlsocket$nt(g_fds.p[client].handle, FIONBIO, &yes) == -1) {
          __closesocket$nt(g_fds.p[client].handle);
          return __winsockerr();
        }
      }
      g_fds.p[client].kind = kFdSocket;
      g_fds.p[client].flags = flags;
      g_fds.p[client].handle = h;
      return client;
    } else {
      return __winsockerr();
    }
  }
}
