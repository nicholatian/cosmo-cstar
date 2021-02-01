#ifndef COSMOPOLITAN_LIBC_NT_STRUCT_WIN32FILEATTRIBUTEDATA_H_
#define COSMOPOLITAN_LIBC_NT_STRUCT_WIN32FILEATTRIBUTEDATA_H_
#include "libc/nt/struct/filetime.h"
#if !(__ASSEMBLER__ + __LINKER__ + 0)

struct NtWin32FileAttributeData {
  uint32_t dwFileAttributes; /* ←NtFileFlagAndAttributes */
  struct NtFileTime ftCreationTime;
  struct NtFileTime ftLastAccessTime;
  struct NtFileTime ftLastWriteTime;
  uint32_t nFileSizeHigh;
  uint32_t nFileSizeLow;
};

#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */
#endif /* COSMOPOLITAN_LIBC_NT_STRUCT_WIN32FILEATTRIBUTEDATA_H_ */
