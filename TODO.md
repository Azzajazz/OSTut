# TODO (in no particular order. Pick what's helpful)

## Outstanding tasks

### High priority

- Make sure kernel code is debuggable
- Switch to protected mode
- Kernel code that actually does something (with VGA, for example)

### Low priority

- Be flexible in how many sectors we load from floppy disk so that the kernel code isn't truncated
- Look at how to enumerate high memory
- Pass parameters from make into build.sh files to avoid duplication
- See if we can jump to 32-bit code while in Real Mode. If not, we need to switch to PM in the bootloader.

## Tasks in progress (and subtask breakdown):

- __BLOCKED__ Informative bootloader messages
  - Report enumerated memory
  - Report switch to protected mode
  - Report any failures
- __BLOCKED__ Firm up memory map (how does this work when we don't necessarily know the memory layout?)
  - Preliminary high memory map (blocked on enumerating high memory and how these two will work together)

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
