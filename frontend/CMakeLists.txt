cmake_minimum_required(VERSION 3.0.0)
project(quik4_frontend)

set(SRC
    debug.cpp
    frontend.cpp
    lex.cpp
    llir_writer.cpp
)

add_library(qkparser STATIC ${SRC})

