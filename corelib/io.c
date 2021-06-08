extern void syscall_str4(int code, int fd, const char *buf, int size);

void _ok_println(const char *line)
{
    int index = 0;
    char c = line[index];
    while (c) {
        ++index;
        c = line[index];
    }
    
    syscall_str4(1, 1, line, index);
    syscall_str4(1, 1, "\n", 1);
}

