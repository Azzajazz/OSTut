#! /bin/bash

#TODO: Take these as arguments?
build_dir=../../build
toolchain_dir=../../toolchain

$toolchain_dir/nasm -f elf -F dwarf -g -o $build_dir/boot.o boot.asm
$toolchain_dir/ld -T link.lds -m elf_i386 -o $build_dir/boot.elf $build_dir/boot.o
objcopy -O binary $build_dir/boot.elf $build_dir/boot.bin
