# TODO (in no particular order. Pick what's helpful)

## Outstanding tasks

### High priority

- Keyboard driver
- File system driver
- ELF loading
- Read up on PCI interface
- Be flexible in how many sectors we load from floppy disk so that the kernel code isn't truncated

### Low priority

- Address some kernel TODOs
- PM bootloader logging
- Pass parameters from make into build.sh files to avoid duplication
- Portability of A20 gate
- Remove `LOAD segment with RWX permissions` warning when linking kernel
- Remove `.note.GNU_stack` warning when linking kernel
- Report switch to protected mode
- Firm up memory map (how does this work when we don't necessarily know the memory layout?)
- Assess whether we really need to vendor the toolchain
- Memory index type so that we don't assume 32-bit addresses
- Pointer format printing
- Make interrupt and exception handlers give more information

## Tasks in progress (and subtask breakdown):

- __ONGOING__ PIC driver
  - __DONE__ Initialisation code through ICWs
  - __ONGOING__ Also OCWs

- __ONGOING__ VGA driver
  - Minimal stuff we need:
    - Setting and reading registers
    - Mode setting (text mode for now, but graphics mode later)
    - Text mode functions

## Finished

Note: This is not a complete list. Items will be deleted when this file gets too long.
This exists mostly for my own benefit, so that I can see progress.

- Verify kernel code is stored on disk correctly. Kernel code is stored immediately after the boot sector for now, but in future it may be stored as a FAT12 file on floppy. TBD.
- Start technical information doc
- Check low memory layout for qemu
- Separate bootloader and kernel code + Makefiles (Do we want a unity build?)
- Write minimal kernel code in C
- Compile with clang freestanding
- Set up version control
- 16-bit RM integer printing (decimal)
- Restricted RM format printing
- RM logging? PM logging?
- Logging prefixes
- Make sure `build.sh` scripts are always run via top-level make file
- Multi-stage bootloader
- Enumerate memory in bootloader
- Switch to protected mode
- Set up data segment registers for PM
- Kernel code that actually does something (with VGA, for example)
- Make sure kernel code is debuggable
- Sized string data type
- Protected mode print functions with format printing
  - u8, u16, u32, u64 and i8, etc. equivalents for format printing
  - Hexadecimal format printing
- Interrupts

## Useful information
- Gate descriptor: Page 57 of 486 datasheet
- Reserved vector assignments: page 45 of 486 datasheet
- PIC: Chapter 12 of the 420EX datasheet
- Programmable interrupt: page 84
