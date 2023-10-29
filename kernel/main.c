#include "defs.h"
#include "riscv.h"

void hello_world() {
  int n = 15;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[15] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W',
                       'o', 'r', 'l', 'd', '!', CR,  LF};
  for (int i = 0; i < n; i++) {
    uartputc_sync(hw_chars[i]);
  }
}

void reached_to_bad() {
  int n = 7;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[7] = {'E', 'R', 'R', 'O', 'R', CR, LF};
  for (int i = 0; i < n; i++) {
    uartputc_sync(hw_chars[i]);
  }
}

void evil_access_machine_register() {
  r_mstatus();
  reached_to_bad();
}

int cpuid() { return r_tp(); }

void main() {
  if (cpuid() == 0) {
    hello_world();
    evil_access_machine_register();
  }
}
