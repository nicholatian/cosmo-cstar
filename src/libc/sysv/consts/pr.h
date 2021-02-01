#ifndef COSMOPOLITAN_LIBC_SYSV_CONSTS_PR_H_
#define COSMOPOLITAN_LIBC_SYSV_CONSTS_PR_H_
#include "libc/runtime/symbolic.h"
#if !(__ASSEMBLER__ + __LINKER__ + 0)
COSMOPOLITAN_C_START_

hidden extern const long PR_CAPBSET_DROP;
hidden extern const long PR_CAPBSET_READ;
hidden extern const long PR_CAP_AMBIENT;
hidden extern const long PR_CAP_AMBIENT_CLEAR_ALL;
hidden extern const long PR_CAP_AMBIENT_IS_SET;
hidden extern const long PR_CAP_AMBIENT_LOWER;
hidden extern const long PR_CAP_AMBIENT_RAISE;
hidden extern const long PR_ENDIAN_BIG;
hidden extern const long PR_ENDIAN_LITTLE;
hidden extern const long PR_ENDIAN_PPC_LITTLE;
hidden extern const long PR_FPEMU_NOPRINT;
hidden extern const long PR_FPEMU_SIGFPE;
hidden extern const long PR_FP_EXC_ASYNC;
hidden extern const long PR_FP_EXC_DISABLED;
hidden extern const long PR_FP_EXC_DIV;
hidden extern const long PR_FP_EXC_INV;
hidden extern const long PR_FP_EXC_NONRECOV;
hidden extern const long PR_FP_EXC_OVF;
hidden extern const long PR_FP_EXC_PRECISE;
hidden extern const long PR_FP_EXC_RES;
hidden extern const long PR_FP_EXC_SW_ENABLE;
hidden extern const long PR_FP_EXC_UND;
hidden extern const long PR_FP_MODE_FR;
hidden extern const long PR_FP_MODE_FRE;
hidden extern const long PR_GET_CHILD_SUBREAPER;
hidden extern const long PR_GET_DUMPABLE;
hidden extern const long PR_GET_ENDIAN;
hidden extern const long PR_GET_FPEMU;
hidden extern const long PR_GET_FPEXC;
hidden extern const long PR_GET_FP_MODE;
hidden extern const long PR_GET_KEEPCAPS;
hidden extern const long PR_GET_NAME;
hidden extern const long PR_GET_NO_NEW_PRIVS;
hidden extern const long PR_GET_PDEATHSIG;
hidden extern const long PR_GET_SECCOMP;
hidden extern const long PR_GET_SECUREBITS;
hidden extern const long PR_GET_SPECULATION_CTRL;
hidden extern const long PR_GET_THP_DISABLE;
hidden extern const long PR_GET_TID_ADDRESS;
hidden extern const long PR_GET_TIMERSLACK;
hidden extern const long PR_GET_TIMING;
hidden extern const long PR_GET_TSC;
hidden extern const long PR_GET_UNALIGN;
hidden extern const long PR_MCE_KILL;
hidden extern const long PR_MCE_KILL_CLEAR;
hidden extern const long PR_MCE_KILL_DEFAULT;
hidden extern const long PR_MCE_KILL_EARLY;
hidden extern const long PR_MCE_KILL_GET;
hidden extern const long PR_MCE_KILL_LATE;
hidden extern const long PR_MCE_KILL_SET;
hidden extern const long PR_MPX_DISABLE_MANAGEMENT;
hidden extern const long PR_MPX_ENABLE_MANAGEMENT;
hidden extern const long PR_SET_CHILD_SUBREAPER;
hidden extern const long PR_SET_DUMPABLE;
hidden extern const long PR_SET_ENDIAN;
hidden extern const long PR_SET_FPEMU;
hidden extern const long PR_SET_FPEXC;
hidden extern const long PR_SET_FP_MODE;
hidden extern const long PR_SET_KEEPCAPS;
hidden extern const long PR_SET_MM;
hidden extern const long PR_SET_MM_ARG_END;
hidden extern const long PR_SET_MM_ARG_START;
hidden extern const long PR_SET_MM_AUXV;
hidden extern const long PR_SET_MM_BRK;
hidden extern const long PR_SET_MM_END_CODE;
hidden extern const long PR_SET_MM_END_DATA;
hidden extern const long PR_SET_MM_ENV_END;
hidden extern const long PR_SET_MM_ENV_START;
hidden extern const long PR_SET_MM_EXE_FILE;
hidden extern const long PR_SET_MM_MAP;
hidden extern const long PR_SET_MM_MAP_SIZE;
hidden extern const long PR_SET_MM_START_BRK;
hidden extern const long PR_SET_MM_START_CODE;
hidden extern const long PR_SET_MM_START_DATA;
hidden extern const long PR_SET_MM_START_STACK;
hidden extern const long PR_SET_NAME;
hidden extern const long PR_SET_NO_NEW_PRIVS;
hidden extern const long PR_SET_PDEATHSIG;
hidden extern const long PR_SET_PTRACER;
hidden extern const long PR_SET_PTRACER_ANY;
hidden extern const long PR_SET_SECCOMP;
hidden extern const long PR_SET_SECUREBITS;
hidden extern const long PR_SET_SPECULATION_CTRL;
hidden extern const long PR_SET_THP_DISABLE;
hidden extern const long PR_SET_TIMERSLACK;
hidden extern const long PR_SET_TIMING;
hidden extern const long PR_SET_TSC;
hidden extern const long PR_SET_UNALIGN;
hidden extern const long PR_SPEC_DISABLE;
hidden extern const long PR_SPEC_ENABLE;
hidden extern const long PR_SPEC_FORCE_DISABLE;
hidden extern const long PR_SPEC_NOT_AFFECTED;
hidden extern const long PR_SPEC_PRCTL;
hidden extern const long PR_SPEC_STORE_BYPASS;
hidden extern const long PR_TASK_PERF_EVENTS_DISABLE;
hidden extern const long PR_TASK_PERF_EVENTS_ENABLE;
hidden extern const long PR_TIMING_STATISTICAL;
hidden extern const long PR_TIMING_TIMESTAMP;
hidden extern const long PR_TSC_ENABLE;
hidden extern const long PR_TSC_SIGSEGV;
hidden extern const long PR_UNALIGN_NOPRINT;
hidden extern const long PR_UNALIGN_SIGBUS;

COSMOPOLITAN_C_END_
#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */

#define PR_CAPBSET_DROP             SYMBOLIC(PR_CAPBSET_DROP)
#define PR_CAPBSET_READ             SYMBOLIC(PR_CAPBSET_READ)
#define PR_CAP_AMBIENT              SYMBOLIC(PR_CAP_AMBIENT)
#define PR_CAP_AMBIENT_CLEAR_ALL    SYMBOLIC(PR_CAP_AMBIENT_CLEAR_ALL)
#define PR_CAP_AMBIENT_IS_SET       SYMBOLIC(PR_CAP_AMBIENT_IS_SET)
#define PR_CAP_AMBIENT_LOWER        SYMBOLIC(PR_CAP_AMBIENT_LOWER)
#define PR_CAP_AMBIENT_RAISE        SYMBOLIC(PR_CAP_AMBIENT_RAISE)
#define PR_ENDIAN_BIG               SYMBOLIC(PR_ENDIAN_BIG)
#define PR_ENDIAN_LITTLE            SYMBOLIC(PR_ENDIAN_LITTLE)
#define PR_ENDIAN_PPC_LITTLE        SYMBOLIC(PR_ENDIAN_PPC_LITTLE)
#define PR_FPEMU_NOPRINT            SYMBOLIC(PR_FPEMU_NOPRINT)
#define PR_FPEMU_SIGFPE             SYMBOLIC(PR_FPEMU_SIGFPE)
#define PR_FP_EXC_ASYNC             SYMBOLIC(PR_FP_EXC_ASYNC)
#define PR_FP_EXC_DISABLED          SYMBOLIC(PR_FP_EXC_DISABLED)
#define PR_FP_EXC_DIV               SYMBOLIC(PR_FP_EXC_DIV)
#define PR_FP_EXC_INV               SYMBOLIC(PR_FP_EXC_INV)
#define PR_FP_EXC_NONRECOV          SYMBOLIC(PR_FP_EXC_NONRECOV)
#define PR_FP_EXC_OVF               SYMBOLIC(PR_FP_EXC_OVF)
#define PR_FP_EXC_PRECISE           SYMBOLIC(PR_FP_EXC_PRECISE)
#define PR_FP_EXC_RES               SYMBOLIC(PR_FP_EXC_RES)
#define PR_FP_EXC_SW_ENABLE         SYMBOLIC(PR_FP_EXC_SW_ENABLE)
#define PR_FP_EXC_UND               SYMBOLIC(PR_FP_EXC_UND)
#define PR_FP_MODE_FR               SYMBOLIC(PR_FP_MODE_FR)
#define PR_FP_MODE_FRE              SYMBOLIC(PR_FP_MODE_FRE)
#define PR_GET_CHILD_SUBREAPER      SYMBOLIC(PR_GET_CHILD_SUBREAPER)
#define PR_GET_DUMPABLE             SYMBOLIC(PR_GET_DUMPABLE)
#define PR_GET_ENDIAN               SYMBOLIC(PR_GET_ENDIAN)
#define PR_GET_FPEMU                SYMBOLIC(PR_GET_FPEMU)
#define PR_GET_FPEXC                SYMBOLIC(PR_GET_FPEXC)
#define PR_GET_FP_MODE              SYMBOLIC(PR_GET_FP_MODE)
#define PR_GET_KEEPCAPS             SYMBOLIC(PR_GET_KEEPCAPS)
#define PR_GET_NAME                 SYMBOLIC(PR_GET_NAME)
#define PR_GET_NO_NEW_PRIVS         SYMBOLIC(PR_GET_NO_NEW_PRIVS)
#define PR_GET_PDEATHSIG            SYMBOLIC(PR_GET_PDEATHSIG)
#define PR_GET_SECCOMP              SYMBOLIC(PR_GET_SECCOMP)
#define PR_GET_SECUREBITS           SYMBOLIC(PR_GET_SECUREBITS)
#define PR_GET_SPECULATION_CTRL     SYMBOLIC(PR_GET_SPECULATION_CTRL)
#define PR_GET_THP_DISABLE          SYMBOLIC(PR_GET_THP_DISABLE)
#define PR_GET_TID_ADDRESS          SYMBOLIC(PR_GET_TID_ADDRESS)
#define PR_GET_TIMERSLACK           SYMBOLIC(PR_GET_TIMERSLACK)
#define PR_GET_TIMING               SYMBOLIC(PR_GET_TIMING)
#define PR_GET_TSC                  SYMBOLIC(PR_GET_TSC)
#define PR_GET_UNALIGN              SYMBOLIC(PR_GET_UNALIGN)
#define PR_MCE_KILL                 SYMBOLIC(PR_MCE_KILL)
#define PR_MCE_KILL_CLEAR           SYMBOLIC(PR_MCE_KILL_CLEAR)
#define PR_MCE_KILL_DEFAULT         SYMBOLIC(PR_MCE_KILL_DEFAULT)
#define PR_MCE_KILL_EARLY           SYMBOLIC(PR_MCE_KILL_EARLY)
#define PR_MCE_KILL_GET             SYMBOLIC(PR_MCE_KILL_GET)
#define PR_MCE_KILL_LATE            SYMBOLIC(PR_MCE_KILL_LATE)
#define PR_MCE_KILL_SET             SYMBOLIC(PR_MCE_KILL_SET)
#define PR_MPX_DISABLE_MANAGEMENT   SYMBOLIC(PR_MPX_DISABLE_MANAGEMENT)
#define PR_MPX_ENABLE_MANAGEMENT    SYMBOLIC(PR_MPX_ENABLE_MANAGEMENT)
#define PR_SET_CHILD_SUBREAPER      SYMBOLIC(PR_SET_CHILD_SUBREAPER)
#define PR_SET_DUMPABLE             SYMBOLIC(PR_SET_DUMPABLE)
#define PR_SET_ENDIAN               SYMBOLIC(PR_SET_ENDIAN)
#define PR_SET_FPEMU                SYMBOLIC(PR_SET_FPEMU)
#define PR_SET_FPEXC                SYMBOLIC(PR_SET_FPEXC)
#define PR_SET_FP_MODE              SYMBOLIC(PR_SET_FP_MODE)
#define PR_SET_KEEPCAPS             SYMBOLIC(PR_SET_KEEPCAPS)
#define PR_SET_MM                   SYMBOLIC(PR_SET_MM)
#define PR_SET_MM_ARG_END           SYMBOLIC(PR_SET_MM_ARG_END)
#define PR_SET_MM_ARG_START         SYMBOLIC(PR_SET_MM_ARG_START)
#define PR_SET_MM_AUXV              SYMBOLIC(PR_SET_MM_AUXV)
#define PR_SET_MM_BRK               SYMBOLIC(PR_SET_MM_BRK)
#define PR_SET_MM_END_CODE          SYMBOLIC(PR_SET_MM_END_CODE)
#define PR_SET_MM_END_DATA          SYMBOLIC(PR_SET_MM_END_DATA)
#define PR_SET_MM_ENV_END           SYMBOLIC(PR_SET_MM_ENV_END)
#define PR_SET_MM_ENV_START         SYMBOLIC(PR_SET_MM_ENV_START)
#define PR_SET_MM_EXE_FILE          SYMBOLIC(PR_SET_MM_EXE_FILE)
#define PR_SET_MM_MAP               SYMBOLIC(PR_SET_MM_MAP)
#define PR_SET_MM_MAP_SIZE          SYMBOLIC(PR_SET_MM_MAP_SIZE)
#define PR_SET_MM_START_BRK         SYMBOLIC(PR_SET_MM_START_BRK)
#define PR_SET_MM_START_CODE        SYMBOLIC(PR_SET_MM_START_CODE)
#define PR_SET_MM_START_DATA        SYMBOLIC(PR_SET_MM_START_DATA)
#define PR_SET_MM_START_STACK       SYMBOLIC(PR_SET_MM_START_STACK)
#define PR_SET_NAME                 SYMBOLIC(PR_SET_NAME)
#define PR_SET_NO_NEW_PRIVS         SYMBOLIC(PR_SET_NO_NEW_PRIVS)
#define PR_SET_PDEATHSIG            SYMBOLIC(PR_SET_PDEATHSIG)
#define PR_SET_PTRACER              SYMBOLIC(PR_SET_PTRACER)
#define PR_SET_PTRACER_ANY          SYMBOLIC(PR_SET_PTRACER_ANY)
#define PR_SET_SECCOMP              SYMBOLIC(PR_SET_SECCOMP)
#define PR_SET_SECUREBITS           SYMBOLIC(PR_SET_SECUREBITS)
#define PR_SET_SPECULATION_CTRL     SYMBOLIC(PR_SET_SPECULATION_CTRL)
#define PR_SET_THP_DISABLE          SYMBOLIC(PR_SET_THP_DISABLE)
#define PR_SET_TIMERSLACK           SYMBOLIC(PR_SET_TIMERSLACK)
#define PR_SET_TIMING               SYMBOLIC(PR_SET_TIMING)
#define PR_SET_TSC                  SYMBOLIC(PR_SET_TSC)
#define PR_SET_UNALIGN              SYMBOLIC(PR_SET_UNALIGN)
#define PR_SPEC_DISABLE             SYMBOLIC(PR_SPEC_DISABLE)
#define PR_SPEC_ENABLE              SYMBOLIC(PR_SPEC_ENABLE)
#define PR_SPEC_FORCE_DISABLE       SYMBOLIC(PR_SPEC_FORCE_DISABLE)
#define PR_SPEC_NOT_AFFECTED        SYMBOLIC(PR_SPEC_NOT_AFFECTED)
#define PR_SPEC_PRCTL               SYMBOLIC(PR_SPEC_PRCTL)
#define PR_SPEC_STORE_BYPASS        SYMBOLIC(PR_SPEC_STORE_BYPASS)
#define PR_TASK_PERF_EVENTS_DISABLE SYMBOLIC(PR_TASK_PERF_EVENTS_DISABLE)
#define PR_TASK_PERF_EVENTS_ENABLE  SYMBOLIC(PR_TASK_PERF_EVENTS_ENABLE)
#define PR_TIMING_STATISTICAL       SYMBOLIC(PR_TIMING_STATISTICAL)
#define PR_TIMING_TIMESTAMP         SYMBOLIC(PR_TIMING_TIMESTAMP)
#define PR_TSC_ENABLE               SYMBOLIC(PR_TSC_ENABLE)
#define PR_TSC_SIGSEGV              SYMBOLIC(PR_TSC_SIGSEGV)
#define PR_UNALIGN_NOPRINT          SYMBOLIC(PR_UNALIGN_NOPRINT)
#define PR_UNALIGN_SIGBUS           SYMBOLIC(PR_UNALIGN_SIGBUS)

#endif /* COSMOPOLITAN_LIBC_SYSV_CONSTS_PR_H_ */
