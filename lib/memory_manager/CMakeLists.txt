cmake_minimum_required(VERSION 3.26)

project(memory-manager)
add_library(memory-manager STATIC source/MemoryManager.cpp)
add_subdirectory(source)

target_include_directories(memory-manager PUBLIC include)
target_compile_options(memory-manager PRIVATE -Werror -Wall -Wextra -pedantic-errors)