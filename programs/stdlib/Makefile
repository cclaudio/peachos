FILES = ./build/start.asm.o ./build/peachos.asm.o
INCLUDES =

all: $(FILES)
	i686-elf-ld -m elf_i386 -relocatable $(FILES) -o ./stdlib.elf

./build/start.asm.o : ./src/start.asm
	nasm -f elf ./src/start.asm -o ./build/start.asm.o

./build/peachos.asm.o : ./src/peachos.asm
	nasm -f elf ./src/peachos.asm -o ./build/peachos.asm.o

clean:
	rm -f $(FILES)
	rm -f ./stdlib.elf