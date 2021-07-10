[BITS 32]

section .asm

global _start
extern c_start
extern peachos_exit

_start:
    call c_start
    call peachos_exit
    ret