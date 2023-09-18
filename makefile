# Compiler
PREFIX=riscv64-linux-gnu-

# Emulator
QEMU=qemu-system-riscv64
# 仮想マシンのhartの数
# def.hで定義した値と一致させること
# jh7110はSモード非対応なコアが1、Sモード対応なコアが4ある
CPUS=5

K=kernel

K_OBJS = kernel/entry.o # The order of objs is important.
K_SRCS = $(shell ls kernel/*.c)
K_OBJS = $(K_SRCS:%.c=%.o)
K_OBJS += kernel/entry.o

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
	$(PREFIX)objdump -D $@ > debug/kernel.S

u-boot/u-boot.bin:
	export CROSS_COMPILE=$(PREFIX) && $(MAKE) -C u-boot qemu-riscv64_defconfig
	export CROSS_COMPILE=$(PREFIX) && $(MAKE) -C u-boot

# QEMU's "-device loader" option extract ELF binary
# at the address where the ELF binary specified.
qemu: kernel/kernel u-boot/u-boot.bin
	$(QEMU) \
		-machine virt -bios u-boot/u-boot.bin -m 4G -smp $(CPUS) -nographic \
		-global virtio-mmio.force-legacy=false \
		-device loader,file=$<

test: kernel/kernel u-boot/u-boot.bin
	$(QEMU) \
		-machine virt -bios u-boot/u-boot.bin -m 4G -smp $(CPUS) -nographic \
		-global virtio-mmio.force-legacy=false -serial mon:stdio \
		-device loader,file=$< \
		-qmp tcp:localhost:4444,server,wait=off

debug/kernel.S: kernel/kernel
	$(PREFIX)objdump -S $< > $@

qemu: kernel/kernel
	$(QEMU) \
		-machine virt -bios none -kernel $< -m 128M -smp $(CPUS) -nographic \
		-global virtio-mmio.force-legacy=false

clean:
	rm \
		kernel/kernel \
		kernel/*.o \
		kernel/*.d \
		debug/*.S