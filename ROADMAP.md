# Roadmap

## Goals

- Roll-your-own bootloader
- Multiprocessing
- Graphical UI
- Boot off hard disk
- Object based file system
- POSIX compliant (for portability reasons)
- Run on bare metal

## How to get there

1. Roll-your-own bootloader
  - Loading in kernel in an extendable way
  - Enumerating memory
  - Switching to protected mode
  - Jump to kernel
  - ...

2. Shell-based UI
  - Keyboard driver
  - Memory management
  - File system driver (FAT12 while booting from floppy)
  - ELF binary loading
  - Process abstraction
  - Bells and whistles (history, builtin commands, etc)
  - ...

3. Graphical UI
  - VGA driver
  - UI library
  - Font rendering (bitmap, then bezier?)
  - ...

4. Boot off hard disk
  - Research this.

5. Object based file system
  - Research this. (File systems used by Google, Haystack by Facebook, etc)

6. POSIX compliance
  - Build this up over time, just wrote implementation of the standard

7. Multiprocessing
  - Research synchronization primitives at the hardware level (mutex, spin-locks, atomic instructions, etc)
  - Memory protection
  - ...

## Tooling

- Make sure kernel is debuggable always
- Serial port communication
- Unit testing/integration testing from host (research how to do this with qemu)
- Robust build system
- Clean version control
