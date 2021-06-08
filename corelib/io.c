#include <stdint.h>

extern void syscall_str4(int code, int fd, const char *buf, int size);

int strlen(const char *line)
{
    int index = 0;
    char c = line[index];
    while (c) {
        ++index;
        c = line[index];
    }
    return index;
}

void println(const char *line)
{
    int size = strlen(line);
    
    syscall_str4(1, 1, line, size);
    syscall_str4(1, 1, "\n", 1);
}

void print_int(int num)
{
    // For some reason, weird things happen with just 0
    if (num == 0) {
        syscall_str4(1, 1, "0", 1);
        return;
    }

    // Count the number of digits
    int size = 0;
    int num2 = num;
    while (num2 != 0) {
        num2 = num2 / 10;
        ++size;
    }
    
    // Convert to a string
    char buf1[size];
    int index = size - 1;
    num2 = num;
    
    while (num2 != 0) {
        int digit = num2 % 10;
        num2 = num2 / 10;
        buf1[index] = digit + '0';
        --index;
    }
    
    // Print
    syscall_str4(1, 1, (char *)buf1, size);
}

void printf(const char *line, int64_t arg1)
{
    int size = strlen(line);
    char buffer[size];
    int index = 0;
    
    for (int i = 0; i<size; i++) {
        if (line[i] == '%' && line[i+1] == 'd') {
            syscall_str4(1, 1, (char *)buffer, index);
            for (int j = 0; j<index; j++) buffer[j] = 0;
            index = 0;
            
            print_int(arg1);
            ++i;
        } else if (line[i] == '\\' && line[i+1] == 'n') {
            buffer[index] = '\n';
            ++index;
        } else {
            buffer[index] = line[i];
            ++index;
        }
    }
    
    syscall_str4(1, 1, (char *)buffer, index);
}

