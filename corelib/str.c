
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

int stringcmp(const char *str1, const char *str2)
{
    int length = strlen(str1);
    if (length != strlen(str2)) return 0;
    
    for (int i = 0; i<length; i++) {
        if (str1[i] != str2[i]) return 0;
    }
    
    return 1;
}
