/*
 * Memory
 *
 * Author: Claudio Carvalho <claudiodecavalho@gmail.com>
 */

#include <stddef.h>

#include "memory.h"

void *memset(void *ptr, int c, size_t size)
{
	char *c_ptr = (char *) ptr;
	for (int i = 0; i < size; i++)
		c_ptr[i] = (char) c;
	return ptr;
}

int memcmp(void *s1, void *s2, size_t n)
{
	char *c1 = s1;
	char *c2 = s2;

	while (n-- > 0) {
		if (*c1++ != *c2++)
			return c1[-1] < c2[-1] ? -1 : 1;
	}
	return 0;
}

void *memcpy(void *dest, void *src, size_t n)
{
	char *d = dest;
	char *s = src;
	
	while (n--)
		*d++ = *s++;
	
	return dest;
}