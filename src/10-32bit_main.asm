[org 0x7c00] ; bootloader offset
             ; 不设置[bits 32]或者[bits 16]时，为实模式，CPU总是按照16位跳转
    mov bp, 0x9000 ; set the stack
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print ; This will be written after the BIOS messages

    call switch_to_pm
    jmp $ ; this will actually never be executed

%include "src/05-boot_sector_print.asm"
%include "src/09-32bit_gdt.asm"
%include "src/08-32bit_print.asm"
%include "src/10-32bit_switch.asm"
%include "src/05-boot_sector_print_hex.asm"

[bits 32]
BEGIN_PM: ; after the switch we will get here
    mov ebx, MSG_PROT_MODE
    call print_string_pm ; Note that this will be written at the top left corner
    jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode, This is 707", 0

; bootsector
times 510-($-$$) db 0
dw 0xaa55