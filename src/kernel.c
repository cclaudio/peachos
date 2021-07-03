/* kernel.c
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include <stdint.h>
#include <stddef.h>

#include "string/string.h"
#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "disk/streamer.h"
#include "fs/file.h"

static struct paging_4gb_chunk *kernel_chunk = 0;

/*
 * Each character in the screen is uint16_t = 2 bytes
 * byte 1 = ascii char
 * byte 2 = colour
 */
#define MONOCHROMATIC_VIDEO_MEM_BASE 0xB8000

uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
	return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
	video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
	if (c == '\n') {
		terminal_row++;
		terminal_col = 0;
		return;
	}

	terminal_putchar(terminal_col, terminal_row, c, colour);
	terminal_col++;

	if (terminal_col >= VGA_WIDTH) {
		terminal_col = 0;
		terminal_row++;
	}
}

void terminal_initialize(void)
{
	video_mem = (uint16_t *) MONOCHROMATIC_VIDEO_MEM_BASE;
	terminal_row = 0;
	terminal_col = 0;

	/* Clear the screen */
	for (int y = 0; y < VGA_HEIGHT; y++)
		for (int x = 0; x < VGA_WIDTH; x++)
			terminal_putchar(x, y, ' ', 0);
}

void print(const char *str)
{
	size_t len = strlen(str);

	for (int i = 0; i < len; i++)
		terminal_writechar(str[i], 15);
}

void kernel_main(void)
{
	terminal_initialize();
	print("Hello world!\ntest");

	// Initialize the heap
	kheap_init();

	// Initialize the filesystems
	fs_init();

	// Search and initialize the disks
	disk_search_and_init();

	// Initialize the interrupt descriptor table
	idt_init();

	// Setup paging
	kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

	// Switch to kernel paging chunk
	paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

	// Enable paging
	enable_paging();

	// Enable the system interrupts
	enable_interrupts();

	int fd = fopen("0:/hello.txt", "r");
	if (fd) {
		struct file_stat s;
		fstat(fd, &s);
	}

	while (1) {}
}