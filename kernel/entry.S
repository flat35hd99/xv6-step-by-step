        # u-boot's bootelf loads the kernel at 0xB0000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0xB0000000.
.section .text
.global _entry
_entry:
        # Pass hartid.
        # addi a0, tp, 0
        addi sp,sp,-16
        sd   ra,16(sp)
        sd   a0, 8(sp)

        # jump to start() in start.c
        call start
        ld   a0, 8(sp)
        ld   ra,16(sp)
        addi sp,sp,16

        ret

spin:
        j spin
