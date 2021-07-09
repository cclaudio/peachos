FILES = ./build/start.asm.o ./build/peachos.asm.o ./build/stdlib.o ./build/stdio.o
FILES += ./build/peachos.o

INCLUDES = -I./src

FLAGS = -g -nostdlib -nostartfiles -nodefaultlibs
FLAGS += -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce
FLAGS += -Iinc -O0 -fomit-frame-pointer -finline-functions -fno-builtin
FLAGS += -Wno-unused-function -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wall

all: $(FILES)
	i686-elf-ld -m elf_i386 -relocatable $(FILES) -o ./stdlib.elf

./build/start.asm.o : ./src/start.asm
	nasm -f elf ./src/start.asm -o ./build/start.asm.o

./build/peachos.asm.o : ./src/peachos.asm
	nasm -f elf ./src/peachos.asm -o ./build/peachos.asm.o

./build/peachos.o : ./src/peachos.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/peachos.c -o ./build/peachos.o

./build/stdlib.o : ./src/stdlib.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/stdlib.c -o ./build/stdlib.o

./build/stdio.o : ./src/stdio.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/stdio.c -o ./build/stdio.o

clean:
	rm -f $(FILES)
	rm -f ./stdlib.elf