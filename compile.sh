#!/bin/bash

riscv64-linux-gnu-gcc -c -o kernel/entry.o kernel/entry.S
riscv64-linux-gnu-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -fno-stack-protector -fno-pie -no-pie   -c -o kernel/start.o kernel/start.c

riscv64-linux-gnu-ld \
  -z max-page-size=4096 \
  -T kernel/kernel.ld \
  -o kernel/kernel \
  kernel/entry.o kernel/start.o

# Output files for debug
if [ ! -d debug ];then
  mkdir debug
fi
riscv64-linux-gnu-objcopy -O binary kernel/kernel debug/kernel.bin
riscv64-linux-gnu-objdump -D kernel/kernel > debug/kernel.S
