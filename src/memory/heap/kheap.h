/*
 * Kernel heap header
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

void kheap_init(void);
void *kmalloc(size_t size);
void kfree(void *ptr);

#endif // KHEAP_H