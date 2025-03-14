#! /bin/bash

build_dir=../../build
toolchain_dir=../../toolchain

$toolchain_dir/nasm -f elf -F dwarf -g -o $build_dir/entry.o entry.asm
$toolchain_dir/clang -c -ffreestanding -nostdlib -m32 -masm=intel -gdwarf-4 -ggdb3 -o $build_dir/kernel.o kernel.c
$toolchain_dir/ld -T link.lds -m elf_i386 -o $build_dir/kernel.elf $build_dir/kernel.o $build_dir/entry.o
objcopy -O binary $build_dir/kernel.elf $build_dir/kernel.bin
