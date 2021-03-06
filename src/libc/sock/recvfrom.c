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
#include "libc/calls/struct/iovec.h"
#include "libc/dce.h"
#include "libc/nt/winsock.h"
#include "libc/sock/internal.h"
#include "libc/sock/sock.h"
#include "libc/sysv/errfuns.h"

/**
 * Receives data from network.
 *
 * This function blocks unless MSG_DONTWAIT is passed.
 *
 * @param fd is the file descriptor returned by socket()
 * @param buf is where received network data gets copied
 * @param size is the byte capacity of buf
 * @param flags can have MSG_{WAITALL,DONTROUTE,PEEK,OOB}, etc.
 * @param opt_out_srcaddr receives the binary ip:port of the data's origin
 * @param opt_inout_srcaddrsize is srcaddr capacity which gets updated
 * @return number of bytes received, 0 on remote close, or -1 w/ errno
 * @error EINTR, EHOSTUNREACH, ECONNRESET (UDP ICMP Port Unreachable),
 *     EPIPE (if MSG_NOSIGNAL), EMSGSIZE, ENOTSOCK, EFAULT, etc.
 * @asyncsignalsafe
 */
ssize_t recvfrom(int fd, void *buf, size_t size, uint32_t flags,
                 void *opt_out_srcaddr, uint32_t *opt_inout_srcaddrsize) {
  ssize_t got;
  if (!IsWindows()) {
    got = recvfrom$sysv(fd, buf, size, flags, opt_out_srcaddr,
                        opt_inout_srcaddrsize);
    if (opt_out_srcaddr && IsBsd() && got != -1) {
      sockaddr2linux(opt_out_srcaddr);
    }
    return got;
  } else if (__isfdkind(fd, kFdSocket)) {
    return recvfrom$nt(&g_fds.p[fd], (struct iovec[]){{buf, size}}, 1, flags,
                       opt_out_srcaddr, opt_inout_srcaddrsize);
  } else {
    return ebadf();
  }
}
