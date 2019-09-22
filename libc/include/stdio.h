#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
 
#define EOF (-1)
 
#ifdef __cplusplus
extern "C" {    // extern "C" 的作用是让 C++ 编译器将 extern "C" 声明的代码当作 C 语言代码处理，可以避免 C++ 因符号修饰导致代码不能和C语言库中的符号进行链接的问题。
#endif
 
int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
 
#ifdef __cplusplus
}
#endif
 
#endif