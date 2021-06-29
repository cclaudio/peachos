; Paging
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

[BITS 32]

section .asm

global paging_load_directory
global enable_paging

; void paging_load_directory(uint32_t *directory)
paging_load_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
    ret

; void enable_paging(void)
enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    pop ebp
    ret