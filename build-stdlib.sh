#!/bin/bash

CFLAGS=-nostdlib -c

echo "Building standard library..."

as corelib/syscall_x86.asm -o build/syscall_x86.o
cc corelib/sys.c -o build/sys.o $CFLAGS

ar -o liborka_corelib.a
    build/syscall_x86.o \
    build/sys.o

as corelib/x64_start.asm -o build/occ_start.o

echo "Done"

