#include <stdint.h>

extern void syscall2(int call, int arg1);
extern void *syscall6(int call, int arg1, int arg2, int arg3, int arg4, int arg5);

void exit(int code)
{
    syscall2(60, code);
}

int *malloc_int32(int64_t size) {
    // Last arg 0
    size *= sizeof(int);
    return (int *)syscall6(6, 0, size, 3, 34, -1);
}

