#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/**
 * 打印data字符直到遇到EOF返回false，或者没有EOF全部打印后返回true
 */
static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

// restrict表示该指针所指数据只能由该指针修改
int printf(const char* restrict format, ...) {
	va_list parameters;	// 定义了一个指针，指针指向可变参数
	va_start(parameters, format);	// 将parameters指针指向format位置，也就是可变参数前一个参数的位置
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;	// 最多还可写入多少
 
		if (format[0] != '%' || format[1] == '%') {	// 当前字符串第一个位置不是%或者第二个位置是%
			if (format[0] == '%')	// 第二个位置肯定是%
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);	// 获取可变参数的下一个参数，并返回int类型
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}