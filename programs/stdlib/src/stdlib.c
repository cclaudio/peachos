#include "peachos.h"
#include "stdlib.h"

void *malloc(size_t size)
{
    return peachos_malloc(size);
}
void free(void *ptr)
{

}