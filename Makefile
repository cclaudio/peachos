# Makefile
#
# Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

# NOTE: kernel.asm.o has to be the first object of this list, otherwise the
# _start won't be at the beginning of the binary.
FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o
FILES += ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o
FILES += ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o ./build/disk/disk.o
FILES += ./build/string/string.o ./build/fs/pparser.o ./build/disk/streamer.o ./build/fs/file.o
FILES += ./build/fs/fat/fat16.o ./build/gdt/gdt.o ./build/gdt/gdt.asm.o ./build/task/tss.asm.o

INCLUDES = -I./src

FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs
FLAGS += -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce
FLAGS += -Iinc -O0 -fomit-frame-pointer -finline-functions -fno-builtin
FLAGS += -Wno-unused-functions -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wall

# BIOS only handles binaries hence -f bin
all: ./bin/boot.bin ./bin/kernel.bin
	rm -f ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	sudo mount -t vfat ./bin/os.bin /mnt/d
	# Copy a file over
	sudo cp ./hello.txt /mnt/d
	sudo umount /mnt/d

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt/idt.asm.o : ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/gdt/gdt.o : ./src/gdt/gdt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/gdt/gdt.c -o ./build/gdt/gdt.o

./build/gdt/gdt.asm.o : ./src/gdt/gdt.asm
	nasm -f elf -g ./src/gdt/gdt.asm -o ./build/gdt/gdt.asm.o

./build/idt/idt.o : ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o : ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.asm.o : ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/task/tss.asm.o : ./src/task/tss.asm
	nasm -f elf -g ./src/task/tss.asm -o ./build/task/tss.asm.o

./build/memory/heap/heap.o : ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o : ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o

./build/memory/paging/paging.o : ./src/memory/paging/paging.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/paging $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/memory/paging/paging.asm.o : ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

./build/disk/disk.o : ./src/disk/disk.c
		i686-elf-gcc $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o

./build/string/string.o : ./src/string/string.c
		i686-elf-gcc $(INCLUDES) -I./src/string $(FLAGS) -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o

./build/fs/pparser.o : ./src/fs/pparser.c
		i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/fs/pparser.c -o ./build/fs/pparser.o

./build/fs/file.o : ./src/fs/file.c
		i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/fs/file.c -o ./build/fs/file.o

./build/fs/fat/fat16.o : ./src/fs/fat/fat16.c
		i686-elf-gcc $(INCLUDES) -I./src/fs -I./src/fs/fat $(FLAGS) -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.o

./build/disk/streamer.o : ./src/disk/streamer.c
		i686-elf-gcc $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/streamer.c -o ./build/disk/streamer.o

debug:
	gdb -ex "add-symbol-file ./build/kernelfull.o 0x100000" -ex "target remote | qemu-system-i386 -hda ./bin/os.bin -S -gdb stdio"

build_x_compiler:
	echo "The host binutils and gcc were built to Linux, we need to build our own."
	echo "See https://wiki.osdev.org/GCC_Cross-Compiler"

clean:
	rm -rf ./bin/*
	rm -f $(FILES)
