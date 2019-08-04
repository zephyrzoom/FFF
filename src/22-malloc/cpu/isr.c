#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "timer.h"
#include "ports.h"

isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    // Remap the PIC
    // PIC接受IRQ，再转发给CPU，CPU会将IRQ号转换为ISR号，CPU会通过out指令和硬件外设交互
    // PIC一共有2个，每个有8个不同输入，再加一个输出，主PIC的输出连接CPU，辅PIC的输出连接主PIC的二号输入口
    // 默认情况IRQs 0 to 7 are set to interrupts 08h to 0Fh, and IRQs 8 to 15 are set to interrupts 70h to 77h
    // priority level: 0, 1, 2, 8, 9, 10, 11, 12, 13, 14, 15, 3, 4, 5, 6, 7.
    // CPU has reserved interrupts 0-31
    // 可以映射范围，8为单位，为避免cpu保留中断，应该大于等于0x20,multiples of 08h: 00h-07h, 08h-0Fh, 10h-17h, 17h-1Fh. And you probably want to use 20h-27h
    // each PIC has to be programmed separately. You can tell the Master PIC to map IRQs 0-7 to INTs 20h-27h, but IRQs 8-F will still be INTs 70h-77h
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0); 

    // Install the IRQs
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t *r) {
    kprint("received interrupt: ");
    char s[3];
    int_to_ascii(r->int_no, s);
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r->int_no]);
    kprint("\n");
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    // n为IRQ0,IRQ1...IRQ15，这里仅仅注册了IRQ0和IRQ1
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t *r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again 
     * 大于40需要由第二个PIC控制器控制*/
    if (r->int_no >= 40) port_byte_out(0xA0, 0x20); /* slave */
    port_byte_out(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];   // 这个中断号有处理函数的话执行，只有IRQ0和IRQ1有处理函数
        handler(r);
    }
}

void irq_install() {
    /* Enable interruptions */
    asm volatile("sti");
    /* IRQ0: timer */
    init_timer(50);
    /* IRQ1: keyboard */
    init_keyboard();
}