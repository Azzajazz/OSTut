# TODO (in no particular order. Pick what's helpful)

## Outstanding tasks

### High priority

- Protected mode print functions with format printing
- VGA driver
  - Minimal stuff we need:
    - Setting and reading registers
    - Mode setting (text mode for now, but graphics mode later)
    - Text mode functions

### Low priority

- PM logging
  - Way to do this so that we don't need to duplicate it in the bootloader and kernel?
  - Can't reference kernel code from bootloader (except entry point) because linker doesn't define order
    - Well... we could define them in a separate section... but that sounds like more trouble than it's worth
  - Can we reference bootloader code from the kernel? Probably.
  - Ahhhh we could define these functions in C and then link with the kernel and the bootloader.
    - This works and is most flexible, but might make the bootloader bigger than necessary. I'd like to keep it below 2 segments if possible.
- Be flexible in how many sectors we load from floppy disk so that the kernel code isn't truncated
- Pass parameters from make into build.sh files to avoid duplication
- Portability of A20 gate
- Remove `LOAD segment with RWX permissions` warning when linking kernel
- Remove `.note.GNU_stack` warning when linking kernel
- Report switch to protected mode
- Firm up memory map (how does this work when we don't necessarily know the memory layout?)

# Tasks in progress (and subtask breakdown):

- Sized string data type

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
