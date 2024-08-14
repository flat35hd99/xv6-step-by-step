#include "defs.h"
#include "riscv.h"
#include "types.h"
#include "spinlock.h"

// Implemented in main.c
void main();

extern struct spinlock sp;

void reached_to_bad() {
  int n = 7;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[7] = {'E', 'R', 'R', 'O', 'R', CR, LF};

  acquire(&sp);
  for (int i = 0; i < n; i++) {
    uartputc_sync(hw_chars[i]);
  }
  release(&sp);
}

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

  // Disable machine interrupts
  w_mstatus(r_mstatus() & !MSTATUS_MIE);

  // Set trap-vector
  w_mtvec((uint64)reached_to_bad);

  w_mepc((uint64)main);
  asm volatile("mret");
}
