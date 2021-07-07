/**
 * Task
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef TASK_H
#define TASK_H

#include "config.h"
#include "memory/paging/paging.h"

struct interrupt_frame;

struct registers {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
};

struct process;

struct task {
    // The page directory of the task
    struct paging_4gb_chunk *page_directory;

    // The registers of the task when it is not running
    struct registers registers;

    // The process of the task
    struct process *process;

    // The next task in the linked list
    struct task *next;

    // Previous task in the linked list
    struct task *prev;
};

struct task *task_new(struct process *process);
struct task *task_current(void);
struct task *task_get_next(void);
int task_free(struct task *task);

int task_switch(struct task *task);
int task_page(void);

void task_run_first_ever_task(void);
void task_return(struct registers *regs);   // It drops us in the user land
void restore_general_purpose_registers(struct registers *regs);
void user_registers(void);

void task_current_save_state(struct interrupt_frame *frame);
int copy_string_from_task(struct task *task, void *virtual, void *phys, int max);

#endif // TASK_H