        # u-boot's bootelf loads the kernel at 0xB0000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0xB0000000.
.section .text
.global _entry
_entry:
        # set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)
        la sp, stack0
        li t0, 1024*4
        addi t1, tp, 0
        mul t0, t0, t1
        add sp, sp, t0

        # Pass hart id(0..3)
        addi a0, tp, -1

        # jump to start() in start.c
        call start

spin:
        j spin
