# Compiler
PREFIX=riscv64-linux-gnu-

# Emulator
QEMU=qemu-system-riscv64
# 仮想マシンのhartの数
# def.hで定義した値と一致させること
# jh7110はSモード非対応なコアが1、Sモード対応なコアが4ある
CPUS=8

K=kernel

K_OBJS = kernel/entry.o # The order of objs is important.
K_SRCS = $(shell ls kernel/*.c)
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
	if [ ! -d debug ]; then mkdir debug; fi
	$(PREFIX)objdump -S $< > $@

QEMUOPTS =  -machine virt -bios none -m 128M -smp $(CPUS) -nographic
QEMUOPTS += -global virtio-mmio.force-legacy=false

qemu: kernel/kernel
	$(QEMU) $(QEMUOPTS) -kernel $<
		

test: kernel/kernel
	go test

# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

qemu-gdb: kernel/kernel
	$(QEMU) $(QEMUOPTS) -kernel $< -S $(QEMUGDB)

clean:
	rm \
		kernel/kernel \
		kernel/*.o \
		kernel/*.d \
		debug/*.S