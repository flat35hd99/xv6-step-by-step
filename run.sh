#!/bin/bash

QEMU=qemu-system-riscv64
CPUS=3 # start.cで定義

$QEMU \
  -machine virt -bios none -kernel kernel/kernel -m 128M -smp $CPUS -nographic \
  -global virtio-mmio.force-legacy=false
