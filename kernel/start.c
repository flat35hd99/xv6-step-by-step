#include "defs.h"
#include "types.h"

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

int counter = 0;

void hello_world() {
  int n = 15;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[15] = {'B', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', CR, LF};
  for (int i = 0; i < n; i++)
  {
    uartputc_sync(hw_chars[i]);
  }
}

void sleep(int n) {
  for (int i = 0; i < n * 10000000; i++)
  {
    ;
  }
}

// hartid start from 0 to 3
void start(int hartid) {

  // if (hartid == 0)
    hello_world();

  uartputc_sync(65 + hartid);
  // hello_world();
  if (hartid != 0) {
    while (1)
    {
      counter++;
      if (counter % 1000000 == 0)
        uartputc_sync(65 + hartid);
    }
  } else {
    while (counter < 1000000)
    {
      ;
    }
    uartputc_sync(97);
  }

  while (1)
  {
    ;
  }
}
