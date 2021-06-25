# Makefile
#
# Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

# BIOS only handles binaries hence -f bin
all:
	nasm -f bin ./boot.asm -o ./boot.bin

clean:
	rm ./boot.bin
