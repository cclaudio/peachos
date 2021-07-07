/*
 * Interrupt Descriptor Table (IDT)
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include "config.h"
#include "idt/idt.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"
#include "task/task.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

static ISR80H_COMMAND isr80h_commands[PEACHOS_MAX_ISR80H_COMMANDS];

extern void idt_load(void *idtr_desc);
extern void int21h(void);
extern void no_interrupt(void);
extern void isr80h_wrapper(void);

void int21h_handler(void)
{
	print("Keyboard pressed!\n");
	/* Send ACK to the PIC */
	outb(0x20, 0x20);
}

void no_interrupt_handler(void)
{
	/* Send ACK to the PIC */
	outb(0x20, 0x20);
}

void idt_zero(void)
{
	print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void *address)
{
	struct idt_desc *desc = &idt_descriptors[interrupt_no];

	desc->offset_1 = (uint32_t) address & 0x0000FFFF;
	desc->selector = KERNEL_CODE_SELECTOR;
	desc->zero = 0x00;
	/*
	 * Present = 1
	 * Descriptor Privilege Level = 11 (ring 3)
	 * Storage Segment = 0
	 * Gate Type = 0b1110
	 */
	desc->type_attr = 0xEE;
	desc->offset_2 = (uint32_t) address >>  16;
}

void idt_init(void)
{
	memset(idt_descriptors, 0, sizeof(idt_descriptors));
	idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
	idtr_descriptor.base = (uint32_t) idt_descriptors;

	/* Initialize all interrupts to no_interrupt_handler */
	for (int i = 0; i < PEACHOS_TOTAL_INTERRUPTS; i++) {
		idt_set(i, no_interrupt_handler);
	}

	idt_set(0, idt_zero);
	idt_set(0x21, int21h);
	idt_set(0x80, isr80h_wrapper);

	// Load the interrupt descriptor table
	idt_load(&idtr_descriptor);
}

void isr80h_register_command(int command_id, ISR80H_COMMAND command)
{
	if (command_id < 0 || command_id >= PEACHOS_MAX_ISR80H_COMMANDS)
		panic("The command is out of bounds\n");
	
	if (isr80h_commands[command_id])
		panic("You are attempting to overwrite an existing command\n");
	
	isr80h_commands[command_id] = command;
}

void *isr80h_handle_command(int command, struct interrupt_frame *frame)
{
	// Invalid command range
	if (command < 0 || command >= PEACHOS_MAX_ISR80H_COMMANDS)
		return 0;

	ISR80H_COMMAND command_func = isr80h_commands[command];

	// Command doesn't exist
	if (!command_func)
		return 0;

	return command_func(frame);
}

void *isr80h_handler(int command, struct interrupt_frame *frame)
{
	void *res = 0;

	kernel_page();
	task_current_save_state(frame);
	res = isr80h_handle_command(command, frame);
	task_page();

	return res;
}