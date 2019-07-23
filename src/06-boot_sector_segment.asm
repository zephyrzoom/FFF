mov ah, 0x0e ; tty

mov al, [the_secret]
int 0x10 ; we already saw this doesn't work, right?

mov bx, 0x7c0 ; remember, the segment is automatically <<4 for you
mov ds, bx ; ds数据段寄存器
; WARNING: from now on all memory references will be offset by 'ds' implicitly
mov al, [the_secret] ; 实际地址为ds * 0x10 + the_secret
int 0x10

; es是扩展寄存器，需要显示调用
mov al, [es:the_secret]
int 0x10 ; doesn't look right... isn't 'es' currently 0x000?

mov bx, 0x7c0
mov es, bx
mov al, [es:the_secret]
int 0x10


jmp $

the_secret:
    db "X"

times 510 - ($-$$) db 0
dw 0xaa55