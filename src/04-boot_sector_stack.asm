mov ah, 0x0e ; tty mode

; 设置栈的基地址
mov bp, 0x8000 ; this is an address far away from 0x7c00 so that we don't get overwritten
mov sp, bp ; if the stack is empty then sp points to bp

; push操作会压栈一次是一个16位数据，数据会存在低8位
push 'A'
push 'B'
push 'C'

; to show how the stack grows downwards
mov al, [0x7ffe] ; 0x8000 - 2, 2是向上移动两个地址，一个地址中存储一个字节，相当于上移了两个字节，一个ax，输出紧邻bp的A
int 0x10

; however, don't try to access [0x8000] now, because it won't work
; you can only access the stack top so, at this point, only 0x7ffe (look above)
mov al, [0x8000]
int 0x10


; recover our characters using the standard procedure: 'pop'
; We can only pop full words so we need an auxiliary register to manipulate
; the lower byte
pop bx
mov al, bl
int 0x10 ; prints C

pop bx
mov al, bl
int 0x10 ; prints B

pop bx
mov al, bl
int 0x10 ; prints A

; data that has been pop'd from the stack is garbage now
mov al, [0x8000]
int 0x10


jmp $
times 510-($-$$) db 0
dw 0xaa55