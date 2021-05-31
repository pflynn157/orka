cmake_minimum_required(VERSION 3.0.0)
project(orka_frontend)

set(SRC
    debug.cpp
    frontend.cpp
    lex.cpp
)

add_library(ocparser STATIC ${SRC})

