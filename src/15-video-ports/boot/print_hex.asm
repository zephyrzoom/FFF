; receiving the data in 'dx'
; For the examples we'll assume that we're called with dx=0x1234
print_hex:
    pusha

    mov cx, 0 ; our index variable

; Strategy: get the last char of 'dx', then convert to ASCII
; Numeric ASCII values: '0' (ASCII 0x30) to '9' (0x39), so just add 0x30 to byte N.
; For alphabetic characters A-F: 'A' (ASCII 0x41) to 'F' (0x46) we'll add 0x40
; Then, move the ASCII byte to the correct position on the resulting string
hex_loop:
    cmp cx, 4 ; loop 4 times
    je end
    
    ; 1. convert last char of 'dx' to ascii
    mov ax, dx ; we will use 'ax' as our working register
    and ax, 0x000f ; 0x1234 -> 0x0004 by masking first three to zeros。前12位置0保留后4位，结果会送到ax
    add al, 0x30 ; add 0x30 to N to convert it to ASCII "N"， 如果是数字，那么肯定在0x30到0x39之间
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
    jle step2   ; 小于等于是数字
    add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58=7。字母第一个与数字之间最后一个ASCII码差了7

step2:
    ; 2. get the correct position of the string to place our ASCII char
    ; bx <- base address + string length - index of char
    mov bx, HEX_OUT + 5 ; base + length, 0x0000位置加5就是他的最后一位，也就是最后一个0的位置
    sub bx, cx  ; our index variable 计数器从0开始
    mov [bx], al ; copy the ASCII char on 'al' to the position pointed by 'bx'
    ror dx, 4 ; 循环右移4位，dx存储的原始数据

    ; increment index and loop
    add cx, 1
    jmp hex_loop

end:
    ; prepare the parameter and call the function
    ; remember that print receives parameters in 'bx'
    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT:
    db '0x0000',0 ; reserve memory for our new string