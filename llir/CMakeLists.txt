cmake_minimum_required(VERSION 3.0.0)
project(orka_llir)

set(SRC
    debug.cpp
    main.cpp
    
    Amd64/AMD64_Writer.cpp
)

add_executable(llir ${SRC})
