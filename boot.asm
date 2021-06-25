; Peach bootloader
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

ORG 0
BITS 16			; 16 bit architecture

_start:
	jmp short start
	nop

; Place holder for the bios parameter block. If the bios pass this parameter,
; it will not corrupt our code
; https://wiki.osdev.org/FAT
times 33 db 0

start:
	jmp 0x7c0:step2		; Make our code segment 0x7c0

	; Before changing the segment registers we better disable interrupts (cli) to avoid
	; any race condition. Then enable interrupts again (sti)
	;
	; x86 ISA doesn't allow us to move imediate value directo to segment registers
	;
	; BIOS loads us to 0x7c00
	; DS:SI
	; 0x7c0 * 16 = 0x7c00
	; message = DS + offset
step2:
	cli
	mov ax, 0x7c0
	mov ds, ax
	mov es, ax
	mov ax, 0x00
	mov ss, ax
	mov sp, 0x7c00		; Stack grows downwards
	sti

	mov si, message
	call print
	jmp $		; Jump to itself

print:
	mov bx, 0
.loop:
	lodsb
	cmp al, 0
	je .done
	call print_char
	jmp .loop
.done:
	ret

; Call into BIOS interrupt call to print a character in the screen
; int 10/AH=0eh
; http://www.ctyme.com/rbrown.htm
print_char:
	mov ah, 0eh
	int 0x10
	ret

message: db 'Hello World!', 0

; The BIOS will look for a bootable segment, which means 0x55AA in the last two
; bytes of the first 512 bytes segment. Remember that x86 is little endian,
; that's why we swap the bytes.
; You can check this running ndisasm boot.bin
times 510-($ - $$) db 0
dw 0xAA55
