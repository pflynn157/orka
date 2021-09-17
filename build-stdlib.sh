#!/bin/bash

CFLAGS="-nostdlib -c -Wno-builtin-declaration-mismatch"

echo "Building core library..."

as corelib/syscall_x86.asm -o build/syscall_x86.o
cc corelib/sys.c -o build/sys.o $CFLAGS
cc corelib/io.c -o build/io.o $CFLAGS
cc corelib/str.c -o build/str.o $CFLAGS

ar rcs build/liborka_corelib.a \
    build/syscall_x86.o \
    build/sys.o \
    build/io.o \
    build/str.o

as corelib/x64_start.asm -o build/occ_start.o

echo "Building standard library..."

mkdir -p build/stdlib

cc stdlib/io.c -o build/stdlib/io.o $CFLAGS -fPIC

ld \
    -o build/stdlib/liborka.so \
    -shared -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
    -Lbuild -lorka_corelib \
    build/stdlib/io.o

echo "Done"

