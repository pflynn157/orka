cmake_minimum_required(VERSION 3.0.0)
project(quik4)

add_compile_options(-std=c++14)

include_directories(frontend)

add_subdirectory(src)
#add_subdirectory(frontend)

