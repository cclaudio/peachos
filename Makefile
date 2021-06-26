# Makefile
#
# Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

# BIOS only handles binaries hence -f bin
all:
	nasm -f bin ./boot.asm -o ./boot.bin


debug:
	gdb -ex "target remote | qemu-system-x86_64 -hda ./boot.bin -S -gdb stdio"

clean:
	rm ./boot.bin
