#!/bin/bash

echo "Building standard library..."

as corelib/x64_start.asm -o build/occ_start.o

echo "Done"

