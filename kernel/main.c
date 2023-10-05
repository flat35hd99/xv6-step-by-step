#include "defs.h"

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

void main() { hello_world(); }