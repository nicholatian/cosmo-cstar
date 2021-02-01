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
#include "libc/bits/pushpop.h"
#include "libc/calls/internal.h"
#include "libc/calls/ntspawn.h"
#include "libc/macros.h"
#include "libc/nt/enum/filemapflags.h"
#include "libc/nt/enum/pageflags.h"
#include "libc/nt/enum/processcreationflags.h"
#include "libc/nt/memory.h"
#include "libc/nt/process.h"
#include "libc/nt/runtime.h"
#include "libc/nt/struct/processinformation.h"
#include "libc/nt/struct/securityattributes.h"
#include "libc/nt/struct/startupinfo.h"

struct SpawnBlock {
  char16_t cmdline[ARG_MAX];
  char16_t envvars[ARG_MAX];
};

/**
 * Spawns process on Windows NT.
 *
 * This function delegates to CreateProcess() with UTF-8 → UTF-16
 * translation and argv escaping. Please note this will NOT escape
 * command interpreter syntax.
 *
 * @param prog won't be PATH searched
 * @param argv specifies prog arguments
 * @param envp[𝟶,m-2] specifies "foo=bar" environment variables, which
 *     don't need to be passed in sorted order; however, this function
 *     goes faster the closer they are to sorted
 * @param envp[m-1] is NULL
 * @param bInheritHandles means handles already marked inheritable will
 *     be inherited; which, assuming the System V wrapper functions are
 *     being used, should mean (1) all files and sockets that weren't
 *     opened with O_CLOEXEC; and (2) all memory mappings
 * @param opt_out_lpProcessInformation can be used to return process and
 *     thread IDs to parent, as well as open handles that need close()
 * @return 0 on success, or -1 w/ errno
 * @see spawnve() which abstracts this function
 */
textwindows int ntspawn(
    const char *prog, char *const argv[], char *const envp[],
    struct NtSecurityAttributes *opt_lpProcessAttributes,
    struct NtSecurityAttributes *opt_lpThreadAttributes, bool32 bInheritHandles,
    uint32_t dwCreationFlags, const char16_t *opt_lpCurrentDirectory,
    const struct NtStartupInfo *lpStartupInfo,
    struct NtProcessInformation *opt_out_lpProcessInformation) {
  int rc;
  int64_t handle;
  size_t blocksize;
  struct SpawnBlock *block;
  rc = -1;
  block = NULL;
  blocksize = ROUNDUP(sizeof(*block), FRAMESIZE);
  if ((handle = CreateFileMappingNuma(
           -1,
           &(struct NtSecurityAttributes){sizeof(struct NtSecurityAttributes),
                                          NULL, false},
           pushpop(kNtPageReadwrite), 0, blocksize, NULL,
           kNtNumaNoPreferredNode)) &&
      (block =
           MapViewOfFileExNuma(handle, kNtFileMapRead | kNtFileMapWrite, 0, 0,
                               blocksize, NULL, kNtNumaNoPreferredNode))) {
    if (mkntcmdline(block->cmdline, prog, argv) != -1 &&
        mkntenvblock(block->envvars, envp) != -1) {
      if (CreateProcess(NULL, block->cmdline, opt_lpProcessAttributes,
                        opt_lpThreadAttributes, bInheritHandles,
                        dwCreationFlags | kNtCreateUnicodeEnvironment,
                        block->envvars, opt_lpCurrentDirectory, lpStartupInfo,
                        opt_out_lpProcessInformation)) {
        rc = 0;
      } else {
        __winerr();
      }
    }
  } else {
    __winerr();
  }
  if (block) UnmapViewOfFile(block);
  if (handle) CloseHandle(handle);
  return rc;
}
