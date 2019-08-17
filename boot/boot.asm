halt:
    jmp halt

times 510-($-$$) db 0
dw 0xaa51   ; intel和amd都是小端模式，存储的时候低地址是55，高地址是aa