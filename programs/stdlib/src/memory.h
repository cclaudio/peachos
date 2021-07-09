#ifndef PEACHOS_MEMORY_H
#define PEACHOS_MEMORY_H

#include <stddef.h>

void *memset(void *ptr, int c, size_t size);
int memcmp(void *s1, void *s2, size_t n);
void *memcpy(void *dest, void *src, size_t n);

#endif // PEACHOS_MEMORY_H
