# Makefile
#
# Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

# BIOS only handles binaries hence -f bin
all:
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

debug:
	gdb -ex "target remote | qemu-system-x86_64 -hda ./bin/boot.bin -S -gdb stdio"

build_x_compiler:
	echo "The host binutils and gcc were built to Linux, we need to build our own."
	echo "See https://wiki.osdev.org/GCC_Cross-Compiler"

clean:
	rm -rf ./bin/*
