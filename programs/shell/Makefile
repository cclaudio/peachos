FILES = ./build/shell.o
INCLUDES = -I../stdlib/src

FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs
FLAGS += -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce
FLAGS += -Iinc -O0 -fomit-frame-pointer -finline-functions -fno-builtin
FLAGS += -Wno-unused-function -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wall

all: $(FILES)
	i686-elf-gcc -g -T ./linker.ld -o ./shell.elf -ffreestanding -O0 -nostdlib -fpic -g $(FILES) ../stdlib/stdlib.elf

./build/shell.o : ./src/shell.c
	i686-elf-gcc $(INCLUDES) -I./ $(FLAGS) -std=gnu99 -c ./src/shell.c -o ./build/shell.o

clean:
	rm -f $(FILES)
	rm -f ./shell.elf
