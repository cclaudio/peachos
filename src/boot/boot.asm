; PeachOS bootloader
; 
; Author: Claudio Carvalho <claudiodecarvalho@gmail.com>

ORG 0x7c00
BITS 16			; 16 bit architecture

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
	jmp short start
	nop

; Place holder for the bios parameter block. If the bios pass this parameter,
; it will not corrupt our code
; https://wiki.osdev.org/FAT
times 33 db 0

start:
	jmp 0x0:step2

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
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00		; Stack grows downwards
	sti

; Enter protected mode
; https://wiki.osdev.org/Protected_mode
.load_protected:
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:load32	; Jumps to the code selector (CS) load32

; Global Descriptor Table (GDT)
gdt_start:
gdt_null:
	dd 0x0
	dd 0x0

; Offset 0x8
gdt_code:		; CS should point to this
	dw 0xffff	; Segment limit first 0-15 bits
	dw 0		; Base first 0-15 bits
	db 0		; Base 16-23 bits
	db 0x9a		; Access byte
	db 11001111b	; High 4 bit flags and the low 4 bit flags
	db 0		; Base 24-31 bits

; Offset 0x10
gdt_data:		; DS, SS, ES, FS, GS
	dw 0xffff	; Segment limit first 0-15 bits
	dw 0		; Base first 0-15 bits
	db 0		; Base 16-23 bits
	db 0x92		; Access byte
	db 11001111b	; High 4 bit flags and the low 4 bit flags
	db 0		; Base 24-31 bits
	
gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start-1	; size field
	dd gdt_start			; offset field

; Everything below this is considered 32 bits
;
; The BIOS loads only the first sector of the bootable disk, the bootloader
; needs to read more if needed.
;
; In protected mode the BIOS services (e.g. int 0x13) are no longer available,
; we need to implement our own disk read function
[BITS 32]
load32:
	mov eax, 1		; LBA. 0 is the boot sector
	mov ecx, 100		; total number of sectors to load
	mov edi, 0x0100000	; buffer target address (1M address)
	call ata_lba_read
	jmp CODE_SEG:0x0100000

; TODO: Implement this in C
ata_lba_read:
	mov ebx, eax 	; Save the LBA
	; Shift right 24 positions. Send the highest 8 bits (32 - 24) of the
	; LBA to hard disk controler
	shr eax, 24
	or eax, 0xE0	; Select the master drive
	mov dx, 0x1F6
	out dx, al
	; Finished sending the highest 8 bits of the lba

	; Send the total sectors to read
	mov eax, ecx
	mov dx, 0x1F2
	out dx, al
	; Finished sending the total sectors to read

	; Send more bits of the LBA
	mov eax, ebx	; Restore the LBA
	mov dx, 0x1F3
	out dx, al
	; Finished sending more bits of the LBA

	; Send more bits of the LBA
	mov dx, 0x1F4
	mov eax, ebx	; Restore the LBA
	shr eax, 8
	out dx, al
	; Finished sending more bits of the LBA

	; Sending upper 16 bits of the LBA
	mov dx, 0x1F5
	mov eax, ebx	; Restore the LBA
	shr eax, 16
	out dx, al
	; Finished sending the upper 16 bits of the LBA

	mov dx, 0x1F7
	mov al, 0x20
	out dx, al

; Read all sectors into memory
.next_sector:
	push ecx

; Checking if we need to read
.try_again:
	mov dx, 0x1F7
	in al, dx
	test al, 8
	jz .try_again

	; We need to read 256 words at a time
	mov ecx, 256
	mov dx, 0x1F0
	rep insw
	pop ecx
	loop .next_sector
	; End of reading sectors into memory
	ret


; The BIOS will look for a bootable segment, which means 0x55AA in the last two
; bytes of the first 512 bytes segment. Remember that x86 is little endian,
; that's why we swap the bytes.
; You can check this running ndisasm boot.bin
times 510-($ - $$) db 0
dw 0xAA55
