; Kernel boot
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>


; Every thing below this is considered 32 bits and we can no longer access
; BIOS interrupt handlers
[BITS 32]
global _start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

	; Enable the A20 line in order to have access to the 21 bit of any
	; memory access
	; https://wiki.osdev.org/A20
	in al, 0x92
	or al, 2
	out 0x92, al

	jmp $
