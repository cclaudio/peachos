[BITS 32]

section .asm

global print:function
global peachos_getkey:function
global peachos_malloc:function
global peachos_free:function
global peachos_putchar:function
global peachos_process_load_start:function
global peachos_process_get_arguments:function

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

; int peachos_getkey(void)
peachos_getkey:
    push ebp
    mov ebp, esp
    mov eax, 2      ; Command getkey
    int 0x80
    pop ebp
    ret

; void peachos_putchar(char c)
peachos_putchar:
    push ebp
    mov ebp, esp
    mov eax, 3          ; Command putchar
    push dword[ebp+8]   ; Variable "c"
    int 0x80
    add esp, 4
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

; void peachos_free(void *ptr)
peachos_free:
    push ebp
    mov ebp, esp
    mov eax, 5             ; Command free (frees the allocated memory for this process)
    push dword[ebp+8]      ; Variable ptr
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_process_load_start(const char *filename)
peachos_process_load_start:
    push ebp
    mov ebp, esp
    mov eax, 6          ; Comand process load start (start a process)
    push dword[ebp+8]    ; Variable "filename"
    int 0x80
    add esp, 4          ; We return to here only when the process is terminated
    pop ebp
    ret

; void peachos_process_get_arguments(struct process_arguments *arguments)
peachos_process_get_arguments:
    push ebp
    mov ebp, esp
    mov eax, 8          ; Command get the process arguments
    push dword[ebp+8]   ; Variable "arguments"
    int 0x80
    add esp, 4
    pop ebp
    ret