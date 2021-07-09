FILES = ./build/start.asm.o ./build/peachos.asm.o ./build/stdlib.o
INCLUDES = -I./src
FLAGS = -g

all: $(FILES)
	i686-elf-ld -m elf_i386 -relocatable $(FILES) -o ./stdlib.elf

./build/start.asm.o : ./src/start.asm
	nasm -f elf ./src/start.asm -o ./build/start.asm.o

./build/peachos.asm.o : ./src/peachos.asm
	nasm -f elf ./src/peachos.asm -o ./build/peachos.asm.o

./build/stdlib.o : ./src/stdlib.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/stdlib.c -o ./build/stdlib.o

clean:
	rm -f $(FILES)
	rm -f ./stdlib.elf