#include "defs.h"

uint __acquire_spinlock(uint);

void initlock(struct spinlock *lk) {
  lk->locked = 0;
}

void acquire(struct spinlock *lk) {
  while(1) {
    if (lk->locked)
      continue;

    // Accrding to https://gcc.gnu.org/onlinedocs/gcc-4.1.0/gcc/Atomic-Builtins.html
    // __sync_bool_compare_and_swap(type *ptr, type oldval type newval, ...)
    // If the value that ptr point is same with oldval, then swap it newval and return true.
    // If not, don't swap and return false.
    if (__sync_bool_compare_and_swap(&lk->locked, 0, 1))
      break;
  }
}

void release(struct spinlock *lk) {
  if (!__sync_bool_compare_and_swap(&lk->locked, 1, 0)){
    // What!?
    // TODO: panic()
    ;
  }
}
