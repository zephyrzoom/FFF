#include "idt.h"
#include "../kernel/util.h"

void set_idt_gate(int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (u32) &idt;  // 不带&也正确执行，idt本来就是地址啊，为什么多加个&，不明白
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    /* __volatile__不让编译器优化该条指令
     * r是通用寄存器
     * %0-%9表示对应的操作数，在这里%0就是&idt_reg
     */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}