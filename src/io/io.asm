; IO
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

section .asm

global insb
global insw
global outb
global outw

; unsigned char insb(unsigned short port)
insb:
    push ebp
    mov ebp, esp

    xor eax, eax        ; eax = 0
    mov edx, [ebp+8]    ; edx = port
    in al, dx

    pop ebp
    ret

; unsigned short insw(unsigned short port)
insw:
    push ebp
    mov ebp, esp

    xor eax, eax        ; eax = 0
    mov edx, [ebp+8]    ; edx = port
    in ax, dx

    pop ebp
    ret

; void outb(unsigned short port, unsigned char val)
outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]   ; eax = val
    mov edx, [ebp+8]    ; edx = port
    out dx, al

    pop ebp
    ret

; void outw(unsigned short port, unsigned short val)
outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]   ; eax = val
    mov edx, [ebp+8]    ; edx = port
    out dx, ax

    pop ebp
    ret
