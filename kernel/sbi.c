#include "sbi.h"

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

void hello()
{
	char hello[6] = {'h', 'e', 'l', 'l', 'o'};
	for (int i = 0; i < 5; i++)
	{
		sbi_console_putchar(hello[i]);
	}
	// sbi_hart_start();
}

void sbi_hart_start(void) {
	struct sbiret ret;

	unsigned long opacue = 42; // This value will be set at a1 register
	// unsigned long address = 0xA0000000;
	unsigned long address = (unsigned long)&hello;
	// print address
	for (int i = (8 - 1); i >= 0; i--) {
        int n = (address >> (i * 4) & 0xF);
        if (n < 10) sbi_console_putchar(n + 48);
        else        sbi_console_putchar((n - 10) + 65);
	}
	// ret = sbi_ecall(SBI_EXT_HSM_HART_START, 0, address, opacue, 0, 0, 0, 0);
	ret = sbi_ecall(SBI_EXT_HSM, SBI_EXT_HSM_HART_START, 0, address, opacue, 0, 0, 0);
	if (ret.error != 0) {
		char error_msg[6] = "error";
		for (int i = 0; i < 5; i++)
			sbi_console_putchar(error_msg[i]);
	}
}
