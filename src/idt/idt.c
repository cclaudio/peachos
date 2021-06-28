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

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(void *idtr_desc);
extern void int21h(void);
extern void no_interrupt(void);

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

	// Load the interrupt descriptor table
	idt_load(&idtr_descriptor);
}

