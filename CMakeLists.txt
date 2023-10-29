cmake_minimum_required(VERSION 3.26)
project(kittenhook)

set(CMAKE_CXX_STANDARD 23)

add_executable(kittenhook source/main.cpp)
add_subdirectory(lib)

target_link_libraries(kittenhook PRIVATE memory-manager)