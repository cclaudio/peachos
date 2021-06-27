# Makefile
#
# Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

# NOTE: kernel.asm.o has to be the first object of this list, otherwise the
# _start won't be at the beginning of the binary.
FILES = ./build/kernel.asm.o

# BIOS only handles binaries hence -f bin
all: ./bin/boot.bin ./bin/kernel.bin
	rm -f ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

debug:
	gdb -ex "add-symbol-file ./build/kernelfull.o 0x100000" -ex "break _start" -ex "target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio"

build_x_compiler:
	echo "The host binutils and gcc were built to Linux, we need to build our own."
	echo "See https://wiki.osdev.org/GCC_Cross-Compiler"

clean:
	rm -rf ./bin/*
