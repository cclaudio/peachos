#include "peachos.h"
#include "stdlib.h"

char *itoa(int i)
{
    static char text[12];
    char neg;
    int loc;

    loc = 11;
    text[11] = 0;
    neg = 1;

    if (i >= 0) {
        neg = 0;
        i = -i;
    }

    while (i) {
        text[--loc] = '0' - (i % 10);
        i /= 10;
    }

    if (loc == 11)
        text[--loc] = '0';

    if (neg)
        text[--loc] = '-';
    
    return &text[loc];
}

void *malloc(size_t size)
{
    return peachos_malloc(size);
}
void free(void *ptr)
{
    peachos_free(ptr);
}