; Interrupt Descriptor Table
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

section .asm

global idt_load
idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]    ; Move first parameter to ebx
    lidt [ebx]
    pop ebp
    ret