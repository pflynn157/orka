extern void syscall2(int call, int arg1);

void exit(int code)
{
    syscall2(60, code);
}

