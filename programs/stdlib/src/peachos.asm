[BITS 32]

global print:function
global getkey:function
global peachos_malloc:function

; void print(const char *message)
print:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 1      ; Command print
    int 0x80
    add esp, 4      ; Restore the stack pointer
    pop ebp
    ret

; int getkey(void)
getkey:
    push ebp
    mov ebp, esp
    mov eax, 2      ; Command getkey
    int 0x80
    pop ebp
    ret

; void *peachos_malloc(size_t size)
peachos_malloc:
    push ebp
    mov ebp, esp
    mov eax, 4          ; Command malloc (allocates memory for the process)
    push dword[ebp+8]   ; Push argument "size"
    int 0x80
    add esp, 4          ; Restore the stack (argument "size" pushed)
    pop ebp
    ret