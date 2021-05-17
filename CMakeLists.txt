cmake_minimum_required(VERSION 3.0.0)
project(quik4)

add_compile_options(-std=c++14)

include_directories(frontend compiler)

add_subdirectory(frontend)
add_subdirectory(compiler)
add_subdirectory(src)

