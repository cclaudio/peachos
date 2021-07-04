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


struct process {
    // The process id
    uint16_t id;

    char filename[PEACHOS_MAX_PATH];

    // The main process task
    struct task *task;

    // Keep track of the process malloc allocations
    void *allocations[PEACHOS_MAX_PROGRAM_ALLOCATIONS];

    // The physical pointer to the process memory
    void *ptr;

    // The physical pointer to the stack memory
    void *stack;

    // The size of the data pointed to by "ptr"
    uint32_t size;
};

#endif // PROCESS_H