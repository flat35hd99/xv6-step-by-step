#!/bin/bash

riscv64-linux-gnu-gcc -c -o kernel/entry.o kernel/entry.S
riscv64-linux-gnu-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -fno-stack-protector -fno-pie -no-pie   -c -o kernel/start.o kernel/start.c

riscv64-linux-gnu-ld \
  -z max-page-size=4096 \
  -T kernel/kernel.ld \
  -o kernel/kernel \
  kernel/entry.o kernel/start.o

$(QEMU) \
  -machine virt -bios none -kernel $K/kernel -m 128M -smp $(CPUS) -nographic \
  -global virtio-mmio.force-legacy=false \
  -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0
