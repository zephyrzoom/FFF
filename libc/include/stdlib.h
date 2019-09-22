#ifndef _STDLIB_H
#define _STDLIB_H 1
 
#include <sys/cdefs.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
__attribute__((__noreturn__))   // 告诉编译器函数不会返回，这可以用来抑制关于未达到代码路径的错误
void abort(void);
 
#ifdef __cplusplus
}
#endif
 
#endif