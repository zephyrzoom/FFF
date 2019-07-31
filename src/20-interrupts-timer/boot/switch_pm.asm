[bits 16]
switch_to_pm:
    cli ; 1. disable interrupts； 禁止中断
    lgdt [gdt_descriptor] ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1 ; 3. set 32-bit mode bit in cr0, 最后一位设置为1，其他位不变
    mov cr0, eax ; 设置之后就进入32位保护模式了
    jmp CODE_SEG:init_pm ; 4. far jump by using a different segment,  8:init_pm，这里开始是新的偏移，从gdt_start开始算起
    ; loads the value is points to into the GDT (using the lgdt instruction), then loads the segment selectors for the data and code segments. Notice that each GDT entry is 8 bytes, and the kernel code descriptor is the second segment, so it's offset is 0x08. Likewise the kernel data descriptor is the third, so it's offset is 16 = 0x10. Here we move the value 0x10 into the data segment registers ds,es,fd,gs,ss. To change the code segment is slightly different; we must do a far jump. This changes the CS implicitly.

[bits 32]
init_pm: ; we are now using 32-bit instructions
    mov ax, DATA_SEG ; 5. update the segment registers  DATA_SEG=16
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; cs是代码段寄存器
    ; ds是数据段寄存器
    ; ss是堆栈段寄存器
    ; es是扩展段寄存器
    ; fs是标志段寄存器
    ; gs是全局段寄存器

    mov ebp, 0x90000 ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call BEGIN_PM ; 7. Call a well-known label with useful code