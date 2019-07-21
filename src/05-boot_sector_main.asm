[org 0x7c00] ; tell the assembler that our offset is bootsector code

; The main routine makes sure the parameters are ready and then calls the function
mov bx, HELLO
call print

call print_nl

mov bx, IM707
call print
call print_nl

mov bx, GOODBYE
call print

call print_nl

mov dx, 0x1fcA
call print_hex

; that's it! we can hang now
jmp $

; remember to include subroutines below the hang
%include "src/05-boot_sector_print.asm"
%include "src/05-boot_sector_print_hex.asm"


; data
HELLO:
    db 'Hello, World', 0

IM707:
    db 'Im707', 0

GOODBYE:
    db 'Goodbye', 0

; padding and magic number
times 510-($-$$) db 0
dw 0xaa55