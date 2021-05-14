cmake_minimum_required(VERSION 3.0.0)
project(quik4_main)

set(SRC
    main.cpp
)

add_executable(qkc ${SRC})

target_link_libraries(qkc
    qkparser
)

