; Peach bootloader
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

ORG 0x7c00		; BIOS loads us into 0x7c00  
BITS 16			; 16 bit architecture

start:
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
