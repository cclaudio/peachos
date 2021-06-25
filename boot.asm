; Peach bootloader
;
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

ORG 0x7c00		; BIOS loads us into 0x7c00  
BITS 16			; 16 bit architecture

start:
	 ; Call into BIOS interrupt call to print a character in the screen
	 ; int 10/AH=0eh
	 ; http://www.ctyme.com/rbrown.htm
	mov ah, 0eh
	mov al, 'A'
	mov bx, 0
	int 0x10

	jmp $		; Jump to itself

; The BIOS will look for a bootable segment, which means 0x55AA in the last two
; bytes of the first 512 bytes segment. Remember that x86 is little endian,
; that's why we swap the bytes.
; You can check this running ndisasm boot.bin
times 510-($ - $$) db 0
dw 0xAA55
