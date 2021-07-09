#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "peachos.h"

int main(int argc, char **argv)
{
    print("PeachOS v1.0.0\n");

    while (1) {
        char buf[1024];
        print("> ");
        peachos_terminal_readline(buf, sizeof(buf), true);
        peachos_process_load_start(buf);
        print("\n");
    }

    return 0;
}