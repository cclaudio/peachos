#include "peachos.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char **argv)
{
    printf("My %s is %i\n", "age", 89);
    print("Hello how are you?\n");
    print(itoa(8763));
    putchar('Z');

    void *ptr = malloc(512);
    free(ptr);

    char buf[1024];
    peachos_terminal_readline(buf, sizeof(buf), true);
    print(buf);

    while(1) {}

    return 0;
}