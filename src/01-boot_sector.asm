; 无限循环 (e9 fd ff)
loop:
    jmp loop

; $表示当前地址
; $$表示当前section开始的地址
; $-$$也就是当前地址之前有多少个字节
; db定义字节
times 510-($-$$) db 0
; Magic number
dw 0xaa55