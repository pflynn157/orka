cmake_minimum_required(VERSION 3.0.0)
project(quik4_compiler)

set(SRC
    compiler.cpp
)

add_library(qkcompiler STATIC ${SRC})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(qkcompiler ${llvm_libs})

