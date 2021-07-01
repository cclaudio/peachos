/*
 * kernel.h
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define PEACHOS_MAX_PATH 108

void kernel_main(void);
void print(const char *str);

#endif