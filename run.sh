#!/bin/bash

QEMU=qemu-system-riscv64

# 仮想マシンのhartの数
# def.hで定義した値と一致させること
# jh7110はSモード非対応なコアが1、Sモード対応なコアが4ある
CPUS=5

$QEMU \
  -machine virt -bios none -kernel kernel/kernel -m 128M -smp $CPUS -nographic \
  -global virtio-mmio.force-legacy=false
