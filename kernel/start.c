#include "defs.h"
#include "types.h"

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

void hello_world() {
  int n = 15;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[15] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', CR, LF};
  for (int i = 0; i < n; i++)
  {
    uartputc_sync(hw_chars[i]);
  }
}

// hartid start from 0 to 3
void start(int hartid) {

  if (hartid == 0)
    hello_world();

  while (1)
  {
    ;
  }
}
