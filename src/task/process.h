/**
 * Process implementation
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#include "config.h"
#include "task.h"

#define PROCESS_FILE_TYPE_ELF 0
#define PROCESS_FILE_TYPE_BINARY 1
typedef unsigned char PROCESS_FILE_TYPE;

struct process {
    // The process id
    uint16_t id;

    char filename[PEACHOS_MAX_PATH];

    // The main process task
    struct task *task;

    // Keep track of the process malloc allocations
    void *allocations[PEACHOS_MAX_PROGRAM_ALLOCATIONS];

    PROCESS_FILE_TYPE filetype;

    union {
        // The physical pointer to the process memory
        void *ptr;
        struct elf_file *elf_file;
    };

    // The physical pointer to the stack memory
    void *stack;

    // The size of the data pointed to by "ptr"
    uint32_t size;

    struct keyboard_buffer {
        char buffer[PEACHOS_KEYBOARD_BUFFER_SIZE];
        int tail;
        int head;
    } keyboard;
};

int process_switch(struct process *process);
int process_load_switch(const char *filename, struct process **process);
int process_load(const char *filename, struct process **process);
struct process *process_current(void);
struct process *process_get(int process_id);

#endif // PROCESS_H