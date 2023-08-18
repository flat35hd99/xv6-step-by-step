#include "sbi.h"
#include "defs.h"

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
			unsigned long arg1, unsigned long arg2,
			unsigned long arg3, unsigned long arg4,
			unsigned long arg5)
{
	struct sbiret ret;

	register unsigned long a0 asm ("a0") = (unsigned long)(arg0);
	register unsigned long a1 asm ("a1") = (unsigned long)(arg1);
	register unsigned long a2 asm ("a2") = (unsigned long)(arg2);
	register unsigned long a3 asm ("a3") = (unsigned long)(arg3);
	register unsigned long a4 asm ("a4") = (unsigned long)(arg4);
	register unsigned long a5 asm ("a5") = (unsigned long)(arg5);
	register unsigned long a6 asm ("a6") = (unsigned long)(fid);
	register unsigned long a7 asm ("a7") = (unsigned long)(ext);
	asm volatile ("ecall"
		      : "+r" (a0), "+r" (a1)
		      : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
		      : "memory");
	ret.error = a0;
	ret.value = a1;

	return ret;
}

struct sbiret sbi_ecall_2(int ext, int fid, unsigned long arg0,
			unsigned long long arg1, unsigned long arg2,
			unsigned long arg3, unsigned long arg4,
			unsigned long arg5)
{
	struct sbiret ret;

	register unsigned long a0 asm ("a0") = (unsigned long)(arg0);
	register unsigned long a1 asm ("a1") = (unsigned long)(arg1);
	register unsigned long a2 asm ("a2") = (unsigned long)(arg2);
	register unsigned long a3 asm ("a3") = (unsigned long)(arg3);
	register unsigned long a4 asm ("a4") = (unsigned long)(arg4);
	register unsigned long a5 asm ("a5") = (unsigned long)(arg5);
	register unsigned long a6 asm ("a6") = (unsigned long)(fid);
	register unsigned long a7 asm ("a7") = (unsigned long)(ext);
	asm volatile ("ecall"
		      : "+r" (a0), "+r" (a1)
		      : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
		      : "memory");
	ret.error = a0;
	ret.value = a1;

	return ret;
}

void sbi_console_putchar(int ch)
{
	sbi_ecall(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0, ch, 0, 0, 0, 0, 0);
}

int sbi_console_getchar(void)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0, 0, 0, 0, 0);

	return ret.error;
}

void hello_world_2() {
  int n = 15;
  char CR = 0x0D;
  char LF = 0x0A;
  char hw_chars[15] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', CR, LF};
  for (int i = 0; i < n; i++)
  {
    uartputc_sync(hw_chars[i]);
    // sbi_console_putchar(hw_chars[i]);
  }
}

int empty_loop() {
	while(1){;}
}

int sbi_hart_start(unsigned long hartid) {
	struct sbiret ret;

	// unsigned long start_addr = 0xB0000000;
	unsigned long long start_addr = (unsigned long long)&hello_world_2;
	// unsigned long mode = 0x1;

	// a0 hartid
	// a1 saddr
	// a2 arg1
	// smode(start mode)はecallしたときのprivilege mode
	ret = sbi_ecall_2(SBI_EXT_HSM, SBI_EXT_HSM_HART_START, hartid, start_addr, 0, 0, 0, 0);
	// ret = sbi_ecall(SBI_EXT_HSM, SBI_EXT_HSM_HART_START, hartid, start_addr, start_addr, mode, 0, 0);
	
	return ret.error;
}

int sbi_hart_get_status(unsigned long hartid) {
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_HSM, SBI_EXT_HSM_HART_GET_STATUS, hartid, 0, 0, 0, 0, 0);

	if (ret.error) {
		return ret.error;
	} else {
		return ret.value;
	}
}