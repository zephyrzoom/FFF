#include <string.h>

/**
 * 内存移动
 */
void* memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {	// 目标地址比源地址小，从低位到高位移动
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {	// 目标地址比源地址大，从高地址到低地址移动，否则可能会覆盖
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}