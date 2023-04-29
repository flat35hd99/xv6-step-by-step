PREFIX=riscv64-linux-gnu-

all: kernel/kernel

kernel/entry.o: kernel/entry.S
	$(PREFIX)gcc -c -o $@ $<

kernel/start.o: kernel/start.c
	$(PREFIX)gcc \
		-Wall -Werror -O -fno-omit-frame-pointer \
		-ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding \
		-fno-common -nostdlib -mno-relax -I. -fno-stack-protector \
		-fno-pie -no-pie -c -o $@ $<

kernel/kernel: kernel/kernel.ld kernel/entry.o kernel/start.o
	$(PREFIX)ld \
		-z max-page-size=4096 \
		-T kernel/kernel.ld \
		-o kernel/kernel \
		kernel/entry.o kernel/start.o
