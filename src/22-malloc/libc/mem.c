#include "mem.h"

void memory_copy(uint8_t *source, uint8_t *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
uint32_t free_mem_addr = 0x10000;
/* Implementation is just a pointer to some free memory which
 * keeps growing */
uint32_t kmalloc(uint32_t size, int align, uint32_t *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 
     * 如果free_mem_addr小于0x1000则free_mem_addr & 0xFFFFF000就是0，也就是不能小于内核起始地址
    */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;    // 对齐，忽略小于0x1000的内容，保留前5位
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer，增加1000，增加几都可以*/
    return ret;
}