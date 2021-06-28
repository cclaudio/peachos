; Interrupt Descriptor Table
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

section .asm

extern int21h_handler
extern no_interrupt_handler

global idt_load
global int21h
global no_interrupt

idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]    ; Move first parameter to ebx
    lidt [ebx]
    pop ebp
    ret

int21h:
    cli
    pushad      ; push all the GPRs
    call int21h_handler
    popad
    sti
    iret

; We get here in case no interrupt set
no_interrupt:
    cli
    pushad      ; push all the GPRs
    call no_interrupt_handler
    popad
    sti
    iret