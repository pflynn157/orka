#!/bin/bash

CFLAGS="-nostdlib -c -Wno-builtin-declaration-mismatch"

echo "Building standard library..."

as corelib/syscall_x86.asm -o build/syscall_x86.o
cc corelib/sys.c -o build/sys.o $CFLAGS
cc corelib/io.c -o build/io.o $CFLAGS

ar rcs build/liborka_corelib.a \
    build/syscall_x86.o \
    build/sys.o \
    build/io.o

as corelib/x64_start.asm -o build/occ_start.o

echo "Done"

