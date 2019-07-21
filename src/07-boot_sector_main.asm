[org 0x7c00]
    mov bp, 0x8000 ; set the stack safely away from us
    mov sp, bp

    mov bx, 0x9000 ; es:bx = 0x0000:0x9000 = 0x09000， es:bx为读取磁盘文件后的存储位置，确保段地址不要超过64k
    mov dh, 2 ; 读取两个扇区，disk_load中会设置位al
    ; the bios sets 'dl' for our boot disk number，(0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
    ; if you have trouble, use the '-fda' flag: 'qemu -fda file.bin'
    call disk_load

    mov dx, [0x9000] ; retrieve the first loaded word, 0xdada
    call print_hex

    call print_nl

    mov dx, [0x9000 + 512] ; first word from second loaded sector, 0xface
    call print_hex

    jmp $

%include "src/05-boot_sector_print.asm"
%include "src/05-boot_sector_print_hex.asm"
%include "src/07-boot_sector_disk.asm"

; Magic number
times 510 - ($-$$) db 0
dw 0xaa55

; 以上的代码在第一个512B的扇区，下面数据在之后的两个扇区
; boot sector = sector 1 of cyl 0 of head 0 of hdd 0
; from now on = sector 2 ...
times 256 dw 0xdada ; sector 2 = 512 bytes
times 256 dw 0xface ; sector 3 = 512 bytes