; Task
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

[BITS 32]

section .asm

global restore_general_purpose_registers
global task_return
global user_registers

; void task_return(struct registers *regs)
;
; We fake an interrupt here. We push all the data to the stack as
; if it were an interrupt and let the processor use that to execute
; in user land
task_return:
    mov ebp, esp
    ; 1. PUSH THE DATA SEGMENT (SS WILL BE FINE)
    ; 2. PUSH THE STACK ADDRESS
    ; 3. PUSH THE FLAGS
    ; 4. PUSH THE CODE SEGMENT
    ; 5. PUSH IP

    ; Let's access the structure passed to us and access them by offset
    ; See the registers structure
    mov ebx, [ebp+4]
    ; 1. Push the data/stack selector
    push dword [ebx+44]
    ; 2. Push the stack pointer
    push dword [ebx+40]
    ; 3. Push the flags
    pushf
    pop eax
    or eax, 0x200   ; enable interrupts when iret is hit
    push eax
    ; 4. Push the code segment
    push dword [ebx+32]
    ; 5. Push ip
    push dword [ebx+28]

    ; Setup some segment registers
    mov ax, [ebx+44]
    mov ds, ax
    mov es, ax     ; The processor will change the stack for us
    mov fs, ax
    mov gs, ax

    push dword [ebx+4]
    call restore_general_purpose_registers
    add esp, 4

    ; Leave kernel land and execute in user land
    iretd


; void restore_general_purpose_registers(struct registers *regs)
restore_general_purpose_registers:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    mov edi, [ebx]
    mov esi, [ebx+4]
    mov ebp, [ebx+8]
    mov edx, [ebx+16]
    mov ecx, [ebx+20]
    mov eax, [ebx+24]
    mov ebx, [ebx+12]
    pop ebp
    ret

; void user_registers(void)
;
; Change all the segment registers to the user data segment registers
user_registers:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret