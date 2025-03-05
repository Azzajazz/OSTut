BUILDDIR := build
SRCDIR := src

DISKIMG := $(BUILDDIR)/disk.img

BOOTBIN := $(BUILDDIR)/boot.bin
KERNELBIN := $(BUILDDIR)/kernel.bin

QEMUFLAGS = -machine q35 -drive file=$(DISKIMG),format=raw,if=floppy,index=0

.PHONY: qemu-run
qemu-run: build-os
	qemu-system-i386 $(QEMUFLAGS)
	
.PHONY: qemu-debug
qemu-debug: build-os
	qemu-system-i386 $(QEMUFLAGS) -gdb tcp::26000 -S

.PHONY: build-os
build-os: make-build-dir $(DISKIMG)

$(DISKIMG): build_bootloader build_kernel
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOTBIN) of=$@ conv=notrunc bs=512
	dd if=$(KERNELBIN) of=$@ conv=notrunc bs=512 seek=2

.ONESHELL: build_bootloader
.PHONY: build_bootloader
build_bootloader:
	cd $(SRCDIR)/bootloader
	./build.sh

.ONESHELL: build_kernel
.PHONY: build_kernel
build_kernel:
	cd $(SRCDIR)/kernel
	./build.sh

.PHONY: make-build-dir
make-build-dir:
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
