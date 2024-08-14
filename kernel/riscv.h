#ifndef __ASSEMBLER__

#include "types.h"

#define MSTATUS_MPP_MASK (3L << 11)  // Previous mode.
#define MSTATUS_MPP_M (3L << 11)     // riscv-privileged-v1.10 Table 1.1 shows
#define MSTATUS_MPP_S (1L << 11)     // M: 11, S: 01, and U: 00
#define MSTATUS_MPP_U (0L << 11)

#define MSTATUS_MIE (1L << 3)    // machine-mode interrupt enable.

static inline uint64 r_mstatus() {
  uint64 x;
  asm volatile("csrr %0, mstatus" : "=r"(x));
  return x;
}

static inline void w_mstatus(uint64 x) {
  asm volatile("csrw mstatus, %0" : : "r"(x));
}

static inline void w_mepc(uint64 x) {
  asm volatile("csrw mepc, %0" : : "r"(x));
}

static inline void w_mtvec(uint64 x) {
  asm volatile("csrw mtvec, %0" : : "r"(x));
}

static inline uint64 r_mhartid() {
  uint64 x;
  asm volatile("csrr %0, mhartid" : "=r"(x));
  return x;
}

static inline uint64 r_tp() {
  uint64 x;
  asm volatile("mv %0, tp" : "=r"(x));
  return x;
}
static inline void w_tp(uint64 x) { asm volatile("mv tp, %0" ::"r"(x)); }

#endif