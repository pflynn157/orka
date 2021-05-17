cmake_minimum_required(VERSION 3.0.0)
project(quik4_compiler)

set(SRC
    compiler.cpp
    ltac_debug.cpp
)

add_library(qkcompiler STATIC ${SRC})

