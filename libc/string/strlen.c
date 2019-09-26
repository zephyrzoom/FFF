#include <string.h>

// 计算字符串长度
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}