#ifndef PEACHOS_H
#define PEACHOS_H

#include <stddef.h>

void print(const char *message);
int getkey(void);
void *peachos_malloc(size_t size);
void peachos_free(void *ptr);
void peachos_putchar(char c);

#endif // PEACHOS_H