#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void main() {
    isr_install();
    /* Test the interrupts */
    /* 立即数前缀$ */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}