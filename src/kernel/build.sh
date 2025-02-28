#! /bin/bash

build_dir=../../build
toolchain_dir=../../toolchain

$toolchain_dir/clang -c -ffreestanding -nostdlib -m32 -gdwarf-4 -ggdb3 -o $build_dir/kernel.o kernel.c
$toolchain_dir/ld -T link.lds -m elf_i386 -o $build_dir/kernel.elf $build_dir/kernel.o
objcopy -O binary $build_dir/kernel.elf $build_dir/kernel.bin
