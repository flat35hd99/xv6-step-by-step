#include "defs.h"
#include "riscv.h"
#include "spinlock.h"

struct spinlock sp;

void hello_world() {
  int n = 15;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[15] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W',
                       'o', 'r', 'l', 'd', '!', CR,  LF};
  acquire(&sp);
  for (int i = 0; i < n; i++) {
    uartputc_sync(hw_chars[i]);
  }
  release(&sp);
}

// void reached_to_bad() {
//   int n = 7;
//   char CR = 0x0D;
//   char LF = 0x0A;
//   char hw_chars[7] = {'E', 'R', 'R', 'O', 'R', CR, LF};

//   acquire(&sp);
//   for (int i = 0; i < n; i++) {
//     uartputc_sync(hw_chars[i]);
//   }
//   release(&sp);
// }

int cpuid() { return r_tp(); }

volatile static int started = 0;
volatile int counter;

void main() {
  if (cpuid() == 0) {
    uartinit();
    initlock(&sp);
    // hello_world();
    counter = 0;
    __sync_synchronize();
    started = 1;
  }

  while (started == 0)
    ;

  __sync_synchronize();
  hello_world();

  if (cpuid() == 0) {
    int dummy_counter = 0;
    for (int j = 0;j < 100000;j++) {
      dummy_counter++;
    }

    while(counter < 8)
      ;
    acquire(&sp);
    uartputc_sync('S');
    release(&sp);

  }
}
