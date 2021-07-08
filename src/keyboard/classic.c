#include <stdint.h>
#include <stddef.h>

#include "keyboard.h"
#include "classic.h"
#include "io/io.h"
#include "kernel.h"
#include "idt/idt.h"
#include "task/task.h"

static uint8_t keyboard_scan_set_one[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '-', '=',
    0x08, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']',
    0x0d, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', 
    0x00, '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', ',', '.', '/', 0x00, '*',
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0', '.'
};

struct keyboard classic_keyboard = {
    .name = {"Classic"},
    .init = classic_keyboard_init,
};

uint8_t classic_keyboard_scancode_to_char(uint8_t scancode)
{
    size_t size_of_keyboard_set_one;
    char c;

    size_of_keyboard_set_one = sizeof(keyboard_scan_set_one) / sizeof(uint8_t);
    if (scancode > size_of_keyboard_set_one)
        return 0;
    
    c = keyboard_scan_set_one[scancode];
    
    return c;
}

void classic_keyboard_handle_interrupt(void)
{
    uint8_t scancode;
    uint8_t c;

    kernel_page();
    scancode = insb(KEYBOARD_INPUT_PORT);
    insb(KEYBOARD_INPUT_PORT);  // Ignore the rogue byte sent to us

    // We only care about press events. Ignore release.
    if (scancode & CLASSIC_KEYBOARD_KEY_RELEASED)
        return;

    c = classic_keyboard_scancode_to_char(scancode);
    if(c != 0)
        keyboard_push(c);

    task_page();
}

struct keyboard *classic_init(void)
{
    return &classic_keyboard;
}

int classic_keyboard_init(void)
{
    idt_register_interrupt_callback(ISR_KEYBOARD_INTERRUPT, classic_keyboard_handle_interrupt);
    
    // Enable the first PS/2 port
    outb(PS2_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
    return 0;
}