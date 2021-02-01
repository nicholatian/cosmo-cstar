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
#include "libc/bits/safemacros.h"
#include "libc/calls/calls.h"
#include "libc/calls/sigbits.h"
#include "libc/calls/struct/sigaction.h"
#include "libc/calls/struct/stat.h"
#include "libc/dce.h"
#include "libc/errno.h"
#include "libc/fmt/conv.h"
#include "libc/fmt/fmt.h"
#include "libc/log/check.h"
#include "libc/log/log.h"
#include "libc/macros.h"
#include "libc/nt/runtime.h"
#include "libc/paths.h"
#include "libc/runtime/gc.h"
#include "libc/runtime/runtime.h"
#include "libc/sock/sock.h"
#include "libc/stdio/stdio.h"
#include "libc/stdio/temp.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/af.h"
#include "libc/sysv/consts/auxv.h"
#include "libc/sysv/consts/ex.h"
#include "libc/sysv/consts/exit.h"
#include "libc/sysv/consts/f.h"
#include "libc/sysv/consts/fd.h"
#include "libc/sysv/consts/fileno.h"
#include "libc/sysv/consts/inaddr.h"
#include "libc/sysv/consts/ipproto.h"
#include "libc/sysv/consts/o.h"
#include "libc/sysv/consts/poll.h"
#include "libc/sysv/consts/sa.h"
#include "libc/sysv/consts/shut.h"
#include "libc/sysv/consts/sig.h"
#include "libc/sysv/consts/so.h"
#include "libc/sysv/consts/sock.h"
#include "libc/sysv/consts/sol.h"
#include "libc/sysv/consts/w.h"
#include "libc/testlib/testlib.h"
#include "libc/x/x.h"
#include "third_party/getopt/getopt.h"
#include "tool/build/runit.h"

/**
 * @fileoverview Remote test runner daemon.
 * Delivers 10x latency improvement over SSH (100x if Debian defaults)
 *
 * Here's how it handles connections:
 *
 * 1. Receives atomically-written request header, comprised of:
 *
 *   - 4 byte nbo magic = 0xFEEDABEEu
 *   - 1 byte command = kRunitExecute
 *   - 4 byte nbo name length in bytes, e.g. "test1"
 *   - 4 byte nbo executable file length in bytes
 *   - <name bytes> (no NUL terminator)
 *   - <file bytes> (it's binary data)
 *
 * 2. Runs program, after verifying it came from the IP that spawned
 *    this program via SSH. Be sure to only run this over a trusted
 *    physically-wired network. To use this software on untrustworthy
 *    networks, wrap it with stunnel and use your own CA.
 *
 * 3. Sends stdout/stderr fragments, potentially multiple times:
 *
 *   - 4 byte nbo magic = 0xFEEDABEEu
 *   - 1 byte command = kRunitStdout/Stderr
 *   - 4 byte nbo byte length
 *   - <chunk bytes>
 *
 * 4. Sends process exit code:
 *
 *   - 4 byte nbo magic = 0xFEEDABEEu
 *   - 1 byte command = kRunitExit
 *   - 1 byte exit status
 */

#define kLogFile     "o/runitd.log"
#define kLogMaxBytes (2 * 1000 * 1000)

char *g_exepath;
volatile bool g_interrupted;
struct sockaddr_in g_servaddr;
unsigned char g_buf[PAGESIZE];
bool g_daemonize, g_sendready;
int g_timeout, g_devnullfd, g_servfd, g_clifd, g_exefd;

void OnInterrupt(int sig) {
  g_interrupted = true;
}

void OnChildTerminated(int sig) {
  int ws, pid;
  for (;;) {
    if ((pid = waitpid(-1, &ws, WNOHANG)) != -1) {
      if (pid) {
        if (WIFEXITED(ws)) {
          DEBUGF("worker %d exited with %d", pid, WEXITSTATUS(ws));
        } else {
          DEBUGF("worker %d terminated with %s", pid, strsignal(WTERMSIG(ws)));
        }
      } else {
        break;
      }
    } else {
      if (errno == EINTR) continue;
      if (errno == ECHILD) break;
      FATALF("waitpid failed in sigchld");
    }
  }
}

wontreturn void ShowUsage(FILE *f, int rc) {
  fprintf(f, "%s: %s %s\n", "Usage", program_invocation_name,
          "[-d] [-r] [-l LISTENIP] [-p PORT] [-t TIMEOUTMS]");
  exit(rc);
}

void GetOpts(int argc, char *argv[]) {
  int opt;
  g_timeout = RUNITD_TIMEOUT_MS;
  g_servaddr.sin_family = AF_INET;
  g_servaddr.sin_port = htons(RUNITD_PORT);
  g_servaddr.sin_addr.s_addr = INADDR_ANY;
  while ((opt = getopt(argc, argv, "hdrl:p:t:w:")) != -1) {
    switch (opt) {
      case 'd':
        g_daemonize = true;
        break;
      case 'r':
        g_sendready = true;
        break;
      case 't':
        g_timeout = atoi(optarg);
        break;
      case 'p':
        CHECK_NE(0xFFFF, (g_servaddr.sin_port = htons(parseport(optarg))));
        break;
      case 'l':
        CHECK_EQ(1, inet_pton(AF_INET, optarg, &g_servaddr.sin_addr));
        break;
      case 'h':
        ShowUsage(stdout, EXIT_SUCCESS);
        unreachable;
      default:
        ShowUsage(stderr, EX_USAGE);
        unreachable;
    }
  }
}

nodiscard char *DescribeAddress(struct sockaddr_in *addr) {
  char ip4buf[16];
  return xasprintf("%s:%hu",
                   inet_ntop(addr->sin_family, &addr->sin_addr.s_addr, ip4buf,
                             sizeof(ip4buf)),
                   ntohs(addr->sin_port));
}

void StartTcpServer(void) {
  int yes = true;
  uint32_t asize;
  CHECK_NE(-1, (g_servfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)));
  LOGIFNEG1(setsockopt(g_servfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));
  LOGIFNEG1(setsockopt(g_servfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)));
  if (bind(g_servfd, &g_servaddr, sizeof(g_servaddr)) == -1) {
    if (g_servaddr.sin_port != 0) {
      g_servaddr.sin_port = 0;
      StartTcpServer();
      return;
    } else {
      FATALF("bind failed %m");
    }
  }
  CHECK_NE(-1, listen(g_servfd, 10));
  asize = sizeof(g_servaddr);
  CHECK_NE(-1, getsockname(g_servfd, &g_servaddr, &asize));
  CHECK_NE(-1, fcntl(g_servfd, F_SETFD, FD_CLOEXEC));
  LOGF("%s:%s", "listening on tcp", gc(DescribeAddress(&g_servaddr)));
  if (g_sendready) {
    printf("ready %hu\n", ntohs(g_servaddr.sin_port));
    fflush(stdout);
    fclose(stdout);
    dup2(g_devnullfd, stdout->fd);
  }
}

void SendExitMessage(int sock, int rc) {
  unsigned char msg[4 + 1 + 1];
  msg[0 + 0] = (unsigned char)((unsigned)RUNITD_MAGIC >> 030);
  msg[0 + 1] = (unsigned char)((unsigned)RUNITD_MAGIC >> 020);
  msg[0 + 2] = (unsigned char)((unsigned)RUNITD_MAGIC >> 010);
  msg[0 + 3] = (unsigned char)((unsigned)RUNITD_MAGIC >> 000);
  msg[4] = kRunitExit;
  msg[5] = (unsigned char)rc;
  CHECK_EQ(sizeof(msg), send(sock, msg, sizeof(msg), 0));
}

void SendOutputFragmentMessage(int sock, enum RunitCommand kind,
                               unsigned char *buf, size_t size) {
  ssize_t rc;
  size_t sent;
  unsigned char msg[4 + 1 + 4];
  msg[0 + 0] = (unsigned char)((unsigned)RUNITD_MAGIC >> 030);
  msg[0 + 1] = (unsigned char)((unsigned)RUNITD_MAGIC >> 020);
  msg[0 + 2] = (unsigned char)((unsigned)RUNITD_MAGIC >> 010);
  msg[0 + 3] = (unsigned char)((unsigned)RUNITD_MAGIC >> 000);
  msg[4 + 0] = kind;
  msg[5 + 0] = (unsigned char)((unsigned)size >> 030);
  msg[5 + 1] = (unsigned char)((unsigned)size >> 020);
  msg[5 + 2] = (unsigned char)((unsigned)size >> 010);
  msg[5 + 3] = (unsigned char)((unsigned)size >> 000);
  CHECK_EQ(sizeof(msg), send(sock, msg, sizeof(msg), 0));
  while (size) {
    CHECK_NE(-1, (rc = send(sock, buf, size, 0)));
    CHECK_LE((sent = (size_t)rc), size);
    size -= sent;
    buf += sent;
  }
}

void HandleClient(void) {
  const size_t kMinMsgSize = 4 + 1 + 4 + 4;
  const size_t kMaxNameSize = 32;
  const size_t kMaxFileSize = 10 * 1024 * 1024;
  unsigned char *p;
  ssize_t got, wrote;
  struct sockaddr_in addr;
  char *addrstr, *exename;
  sigset_t chldmask, savemask;
  int exitcode, wstatus, child, pipefds[2];
  struct sigaction ignore, saveint, savequit;
  uint32_t addrsize, namesize, filesize, remaining;

  /* read request to run program */
  addrsize = sizeof(addr);
  CHECK_NE(-1, (g_clifd = accept4(g_servfd, &addr, &addrsize, SOCK_CLOEXEC)));
  if (fork()) {
    close(g_clifd);
    return;
  }
  addrstr = gc(DescribeAddress(&addr));
  DEBUGF("%s %s %s", gc(DescribeAddress(&g_servaddr)), "accepted", addrstr);
  got = recv(g_clifd, (p = &g_buf[0]), sizeof(g_buf), 0);
  CHECK_GE(got, kMinMsgSize);
  CHECK_LE(got, sizeof(g_buf));
  CHECK_EQ(RUNITD_MAGIC, read32be(p));
  p += 4, got -= 4;
  CHECK_EQ(kRunitExecute, *p++);
  got--;
  namesize = read32be(p), p += 4, got -= 4;
  filesize = read32be(p), p += 4, got -= 4;
  CHECK_GE(got, namesize);
  CHECK_LE(namesize, kMaxNameSize);
  CHECK_LE(filesize, kMaxFileSize);
  exename = gc(xasprintf("%.*s", namesize, p));
  g_exepath = gc(xasprintf("o/%d.%s", getpid(), basename(exename)));
  LOGF("%s asked we run %`'s (%,u bytes @ %`'s)", addrstr, exename, filesize,
       g_exepath);
  p += namesize, got -= namesize;

  /* write the file to disk */
  remaining = filesize;
  CHECK_NE(-1, (g_exefd = creat(g_exepath, 0700)));
  ftruncate(g_exefd, filesize);
  if (got) {
    CHECK_EQ(got, write(g_exefd, p, got));
    CHECK_LE(got, remaining);
    remaining -= got;
  }
  while (remaining) {
    CHECK_NE(-1, (got = recv(g_clifd, g_buf, sizeof(g_buf), 0)));
    CHECK_LE(got, remaining);
    if (!got) {
      LOGF("%s %s %,u/%,u %s", addrstr, "sent", remaining, filesize,
           "bytes before hangup");
      unlink(g_exepath);
      _exit(0);
    }
    remaining -= got;
    p = &g_buf[0];
    do {
      CHECK_GT((wrote = write(g_exefd, g_buf, got)), 0);
      CHECK_LE(wrote, got);
    } while ((got -= wrote));
  }
  LOGIFNEG1(shutdown(g_clifd, SHUT_RD));
  LOGIFNEG1(close(g_exefd));

  /* run program, tee'ing stderr to both log and client */
  DEBUGF("spawning %s", exename);
  ignore.sa_flags = 0;
  ignore.sa_handler = SIG_IGN;
  LOGIFNEG1(sigemptyset(&ignore.sa_mask));
  LOGIFNEG1(sigaction(SIGINT, &ignore, &saveint));
  LOGIFNEG1(sigaction(SIGQUIT, &ignore, &savequit));
  LOGIFNEG1(sigemptyset(&chldmask));
  LOGIFNEG1(sigaddset(&chldmask, SIGCHLD));
  LOGIFNEG1(sigprocmask(SIG_BLOCK, &chldmask, &savemask));
  CHECK_NE(-1, pipe2(pipefds, O_CLOEXEC));
  CHECK_NE(-1, (child = fork()));
  if (!child) {
    sigaction(SIGINT, &saveint, NULL);
    sigaction(SIGQUIT, &savequit, NULL);
    sigprocmask(SIG_SETMASK, &savemask, NULL);
    dup2(pipefds[1], 2);
    execv(g_exepath, (char *const[]){g_exepath, NULL});
    _exit(127);
  }
  LOGIFNEG1(close(pipefds[1]));
  DEBUGF("communicating %s[%d]", exename, child);
  for (;;) {
    CHECK_NE(-1, (got = read(pipefds[0], g_buf, sizeof(g_buf))));
    if (!got) {
      close(pipefds[0]);
      break;
    }
    fwrite(g_buf, got, 1, stderr);
    SendOutputFragmentMessage(g_clifd, kRunitStderr, g_buf, got);
  }
  while (waitpid(child, &wstatus, 0) == -1) {
    if (errno == EINTR) continue;
    FATALF("waitpid failed");
  }
  if (WIFEXITED(wstatus)) {
    DEBUGF("%s exited with %d", exename, WEXITSTATUS(wstatus));
    exitcode = WEXITSTATUS(wstatus);
  } else {
    DEBUGF("%s terminated with %s", exename, strsignal(WTERMSIG(wstatus)));
    exitcode = 128 + WTERMSIG(wstatus);
  }
  LOGIFNEG1(sigaction(SIGINT, &saveint, NULL));
  LOGIFNEG1(sigaction(SIGQUIT, &savequit, NULL));
  LOGIFNEG1(sigprocmask(SIG_SETMASK, &savemask, NULL));

  /* let client know how it went */
  LOGIFNEG1(unlink(g_exepath));
  SendExitMessage(g_clifd, exitcode);
  LOGIFNEG1(shutdown(g_clifd, SHUT_RDWR));
  LOGIFNEG1(close(g_clifd));
  _exit(0);
}

int Poll(void) {
  int i, wait, evcount;
  struct pollfd fds[1];
TryAgain:
  if (g_interrupted) return 0;
  fds[0].fd = g_servfd;
  fds[0].events = POLLIN;
  wait = MIN(1000, g_timeout);
  evcount = poll(fds, ARRAYLEN(fds), wait);
  if (!evcount) g_timeout -= wait;
  if (evcount == -1 && errno == EINTR) goto TryAgain;
  CHECK_NE(-1, evcount);
  for (i = 0; i < evcount; ++i) {
    CHECK(fds[i].revents & POLLIN);
    HandleClient();
  }
  /* manually do this because of nt */
  while (waitpid(-1, NULL, WNOHANG) > 0) donothing;
  return evcount;
}

int Serve(void) {
  StartTcpServer();
  sigaction(SIGINT, (&(struct sigaction){.sa_handler = (void *)OnInterrupt}),
            NULL);
  sigaction(SIGCHLD,
            (&(struct sigaction){.sa_handler = (void *)OnChildTerminated,
                                 .sa_flags = SA_RESTART}),
            NULL);
  for (;;) {
    if (!Poll() && (!g_timeout || g_interrupted)) break;
  }
  close(g_servfd);
  if (!g_timeout) {
    LOGF("timeout expired, shutting down");
  } else {
    LOGF("got ctrl-c, shutting down");
  }
  return 0;
}

void Daemonize(void) {
  struct stat st;
  if (fork() > 0) _exit(0);
  setsid();
  if (fork() > 0) _exit(0);
  dup2(g_devnullfd, stdin->fd);
  if (!g_sendready) dup2(g_devnullfd, stdout->fd);
  freopen(kLogFile, "ae", stderr);
  if (fstat(fileno(stderr), &st) != -1 && st.st_size > kLogMaxBytes) {
    ftruncate(fileno(stderr), 0);
  }
}

int main(int argc, char *argv[]) {
  showcrashreports();
  /* g_loglevel = kLogDebug; */
  GetOpts(argc, argv);
  CHECK_NE(-1, (g_devnullfd = open("/dev/null", O_RDWR)));
  defer(close_s, &g_devnullfd);
  if (!isdirectory("o")) CHECK_NE(-1, mkdir("o", 0700));
  if (g_daemonize) Daemonize();
  return Serve();
}
