PREFIX=riscv64-linux-gnu-

K=kernel

K_SRCS = $(shell ls kernel/*.c)
K_OBJS += kernel/entry.o
K_OBJS += $(K_SRCS:%.c=%.o)

all: kernel/kernel debug/kernel.S

kernel/entry.o: kernel/entry.S
	$(PREFIX)gcc -c -o $@ $<

kernel/%.o: kernel/%.c
	$(PREFIX)gcc \
		-Wall -Werror -O -fno-omit-frame-pointer \
		-ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding \
		-fno-common -nostdlib -mno-relax -I. -fno-stack-protector \
		-fno-pie -no-pie -c -o $@ $<

kernel/kernel: kernel/kernel.ld $(K_OBJS)
	$(PREFIX)ld \
		-z max-page-size=4096 \
		-T kernel/kernel.ld \
		-o $@ \
		$(K_OBJS)

debug/kernel.S: kernel/kernel
	$(PREFIX)objdump -S $< > $@

clean:
	rm \
		kernel/kernel \
		kernel/*.o \
		kernel/*.d \
		debug/*.S