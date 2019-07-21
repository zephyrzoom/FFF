gdt_start: ; don't remove the labels, they're needed to compute sizes and jumps
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT abbr. Global Descriptor Table
; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code: 
    dw 0xffff    ; segment length, bits 0-15, 最大寻址长度
    dw 0x0       ; segment base, bits 0-15， 段地址起始地址
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits).   
       ; pr:1，必须为1后面的选择器才生效
        ; privl,特权级别 2位， 0为kernel，3为user，特权逐渐减小
          ; s, 描述符类型，为代码或数据段设置
           ; ex, 可执行位， 1为可执行，0为不可执行就是数据选择器
            ; dc, 寻址方向和特权顺序方向，如果是数据选择器，则0为地址增大，1为地址减小；如果是代码选择器，则0为只能执行当前特权级别代码，1则可以执行小于等于当前特权的代码
             ; rw, 允许读写位，代码段可设置读权限，数据段可设置读写权限
              ; ac, 段是否允许访问，设置为0即可，cpu会自动置为1
    db 11001111b ; flags (4 bits) + segment length, bits 16-19. 
       ; gr 粒度，0为1B，1为4B
        ; sz 0为16位模式，1为32位模式
         ; 保留2位 10为64位模式
           ; 0xF最大寻址长度,加上前面的是0xFFFFF,20位，4GB寻址空间
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)

; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start