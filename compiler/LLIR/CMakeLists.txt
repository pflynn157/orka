cmake_minimum_required(VERSION 3.0.0)
project(orka_compiler_llir)

set(SRC
    LLIRCompiler.cpp
    LLIRDebug.cpp
    PASMDebug.cpp
)

add_library(occompiler_llir ${SRC})

