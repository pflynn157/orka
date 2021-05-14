cmake_minimum_required(VERSION 3.0.0)
project(quik4_frontend)

set(SRC
    frontend.cpp
    lex.cpp
)

add_library(qkparser STATIC ${SRC})

