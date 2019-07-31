#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        // 位置有负数则返回当前位置，在当前光标位置打印
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char *message) {
    kprint_at(message, -1, -1);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;   // 默认设置为黑白色

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';    // 在最后一个单元显示E
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;   // 白底红字
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) 
            // 超出之后把所有内容向前一行的内存转存
            memory_copy(get_offset(0, i) + VIDEO_ADDRESS,
                        get_offset(0, i-1) + VIDEO_ADDRESS,
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;    // 减去一行就是最后一行的第一个
        for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;    // 最后一行全设置为空，与' 'WHITE_ON_BLACK效果一样

        offset -= 2 * MAX_COLS; // offset超出了一行大小，那么上面内容scroll后给他减去一行，就可以在最后一行输入了
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell，一个字符占据两个位置*/
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data 
     * 先÷2得到实际字符单元位置
     */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14); // 高8位
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));   // 取到offset的高8位，低8位被清除了
    port_byte_out(REG_SCREEN_CTRL, 15); // 低8位
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff)); // 和ff与可以取到offset的低8位
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = VIDEO_ADDRESS;

    // 用行*列为所有要清楚的显示单元数量
    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

/* 包括了属性单元信息所占的单元
 * 有一个×2的操作，为实际屏幕显示的字符个数
 */
int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
/* 位置÷列数向下取整 */
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
/* offset是×2以后的，MAX_COLS*2是实际的单元数量， offset-行数*2*MAX_COLS是最后一行多余出来的位置数，除以二是字符数*/
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2*MAX_COLS)) / 2; }