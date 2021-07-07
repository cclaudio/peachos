; Interrupt Descriptor Table
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

section .asm

extern int21h_handler
extern no_interrupt_handler
extern isr80h_handler

global idt_load
global int21h
global no_interrupt
global enable_interrupts
global disable_interrupts
global isr80h_wrapper

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret

idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]    ; Move first parameter to ebx
    lidt [ebx]
    pop ebp
    ret

int21h:
    pushad      ; push all the GPRs
    call int21h_handler
    popad
    iret

; We get here in case no interrupt set
no_interrupt:
    pushad      ; push all the GPRs
    call no_interrupt_handler
    popad
    iret

isr80h_wrapper:
    ; INTERRUPT FRAME START
    ; ALREADY PUSHED TO US BY THE PROCESSOR UPON ENTRY TO THIS INTERRUPT
    ; uint32_t ip
    ; uint32_t cs
    ; uint32_t flags
    ; uint32_t sp
    ; uint32_t ss
    ;
    ; Push the general purpose registers to the stack
    pushad

    ; INTERRUPT FRAME END

    ; Push the stack pointer so that we are pointing to the interrupt frame
    push esp
    ; EAX holds our command lets push it to the stack for isr80h_handlers
    push eax
    call isr80h_handler
    mov dword[tmp_res], eax
    add esp, 8          ; Discard the last 2 values we pushed to the stack

    ; Restore general purpose registers for user land
    popad
    mov eax, [tmp_res]
    iretd

section .data

; Inside here is stored the return result from isr80h_handlers
tmp_res: dd 0
