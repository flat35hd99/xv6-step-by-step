#include "defs.h"
#include "types.h"
#include "sbi.h"
// #include "spinlock.h"

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

void hello_world() {
  const int n = 15;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[15] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', CR, LF};
  for (int i = 0; i < n; i++)
  {
    // uartputc_sync(hw_chars[i]);
    sbi_console_putchar(hw_chars[i]);
  }
}

void print_newline() {
  char CR = 0x0D;
  char LF = 0x0A;
  sbi_console_putchar(CR);
  sbi_console_putchar(LF);
}

void start(uint hartid) {
  hello_world();
  // Check before
  for (unsigned long i = 0; i < 8; i++)
  {
    sbi_console_putchar(sbi_hart_get_status(i) + 65);
  }
  print_newline();

  // Start harts
  for (unsigned long i = 0; i < 8; i++)
  {
    if (i == hartid) {
      // Skip starting current hart
      sbi_console_putchar(32);
      continue;
    }
    if (i == 0) {
      // Skip starting hart which does not support S mode.
      sbi_console_putchar(32);
      continue;
    }
    sbi_console_putchar(sbi_hart_start(i) + 65);
  }
  print_newline();

  // Send IPI
  sbi_send_ipi(~hartid , 0);

  // Check after
  for (unsigned long i = 0; i < 8; i++)
  {
    sbi_console_putchar(sbi_hart_get_status(i) + 65);
  }
  print_newline();

  long user_input = 0;
  unsigned long count_result = 0;
  while (1)
  {
    user_input = sbi_console_getchar();
    if (user_input > 0) {
      // sbi_console_putchar(user_input);
      count_result = get_count();
      for (int i = (8 - 1); i >= 0; i--) {
        int n = (count_result >> (i * 4) & 0xF);
        if (n < 10) sbi_console_putchar(n + 48);
        else        sbi_console_putchar((n - 10) + 65);
      }
      print_newline();
    }
  }
}
