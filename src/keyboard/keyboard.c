#include <stddef.h>

#include "keyboard.h"
#include "status.h"
#include "kernel.h"
#include "task/task.h"
#include "classic.h"

static struct keyboard *keyboard_list_head = NULL;
static struct keyboard *keyboard_list_last = NULL;

void keyboard_init(void)
{
    keyboard_insert(classic_init());
}

int keyboard_insert(struct keyboard *keyboard)
{
    if (keyboard->init == NULL)
        return -EINVARG;

    if (keyboard_list_last) {
        keyboard_list_last->next = keyboard;
        keyboard_list_last = keyboard;
    } else {
        keyboard_list_head = keyboard;  
        keyboard_list_last = keyboard;
    }

    return keyboard->init();
}

static int keyboard_get_tail_index(struct process *process)
{
    return process->keyboard.tail % sizeof(process->keyboard.buffer);
}

void keyboard_backspace(struct process *process)
{
    int real_index;

    process->keyboard.tail -= 1;
    real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = 0x00;
}

// Push to the keyboard we are interacting on the screen
void keyboard_push(char c)
{
    struct process *process;
    int real_index;
    
    process = process_current();
    if (!process)
        return;
    
    real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = c;
    process->keyboard.tail++;
}

// Pop from the current task, which may not be the active one
char keyboard_pop(void)
{
    struct process *process;
    int real_index;
    char c;

    if (!task_current())
        return 0;
    
    process = task_current()->process;
    real_index = process->keyboard.head % sizeof(process->keyboard.buffer);
    c  = process->keyboard.buffer[real_index];
    if (c == 0x00)
        return 0; // Nothing to pop

    process->keyboard.buffer[real_index] = 0;
    process->keyboard.head++;
    return c;
}