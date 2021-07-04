 /**
 * Task Implementation
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include "task.h"
#include "kernel.h"
#include "status.h"
#include "process.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"

// The current task that is running
struct task *current_task = NULL;

// Task linked list
struct task *task_tail = NULL;
struct task *task_head = NULL;

struct task *task_current(void)
{
    return current_task;
}

struct task *task_get_next(void)
{
    if (!current_task->next)
        return task_head;

    return current_task->next;
}

static void task_list_remove(struct task *task)
{
    if (task->prev)
        task->prev->next = task->next;

    if (task == task_head)
        task_head = task->next;

    if (task == task_tail)
        task_tail = task->prev;

    if (task == current_task)
        current_task = task_get_next();
}

int task_free(struct task *task)
{
    paging_free_4gb(task->page_directory);
    task_list_remove(task);
    // Finally free the task data
    kfree(task);
    return 0;
}

int task_init(struct task *task, struct process *process)
{
    memset(task, 0, sizeof(struct task));

    // Map the entire 4GB address space to its self (read-only)
    task->page_directory = paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    if (!task->page_directory)
        return -EIO;

    task->registers.ip = PEACHOS_PROGRAM_VIRTUAL_ADDRESS;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.esp = PEACHOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START;

    task->process = process;

    return 0;
}

struct task *task_new(struct process *process)
{
    struct task *task;
    int res = 0;

    task = kzalloc(sizeof(struct task));
    if (!task) {
        res = -ENOMEM;
        goto err_free_mem;
    }

    res = task_init(task, process);
    if (res != PEACHOS_ALL_OK)
        goto err_free_mem;

    if (task_head == NULL) {
        // First task
        task_head = task;
        task_tail = task;
    } else {
        // Append task
        task_tail->next = task;
        task->prev = task_tail;
        task_tail = task;
    }

    return task;

err_free_mem:
    task_free(task);
    return ERROR(res);
}

int task_switch(struct task *task)
{
    current_task = task;
    paging_switch(task->page_directory->directory_entry);
    return 0;
}

// Task page directories
int task_page(void)
{
    user_registers();
    task_switch(current_task);
    return 0;
}

void task_run_first_ever_task(void)
{
    // Not allowed if task is not loaded
    if (!current_task)
        panic("task_run_first_ever_task(): No current task exists!\n");

    task_switch(task_head);
    task_return(&task_head->registers);
}