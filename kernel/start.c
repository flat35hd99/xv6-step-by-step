#include "defs.h"
#include "riscv.h"
#include "types.h"

// Implemented in main.c
void main();

// entry.S needs one stack per CPU.
__attribute__((aligned(16))) char stack0[4096 * NCPU];

// hartid start from 0 to 3
void start() {
  // Set M Previous Privilege mode to Supervisor, for mret.
  unsigned long x = r_mstatus();
  x &= ~MSTATUS_MPP_MASK;
  x |= MSTATUS_MPP_S;
  w_mstatus(x);

  // Keep each CPU's hartid in its tp register, for cpuid().
  int id = r_mhartid();
  w_tp(id);

  w_mepc((uint64)main);
  asm volatile("mret");
}
