        # u-boot's bootelf loads the kernel at 0xB0000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0xB0000000.
.section .text
.global _entry
_entry:
        # set up a stack for C has been completed by SBI.

        # Pass hartid.
        addi a0, tp, 0

        # jump to start() in start.c
        call start

spin:
        j spin
