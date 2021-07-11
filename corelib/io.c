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

// TODO: Move this elsewhere
typedef struct
{
    char *array;
    int size;
} CharArray;

void printCharArray(CharArray array)
{
    syscall_str4(1, 1, array.array, array.size);
}

