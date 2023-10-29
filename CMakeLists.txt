cmake_minimum_required(VERSION 3.26)
project(kittenhook)

set(CMAKE_CXX_STANDARD 23)

add_executable(kittenhook source/main.cpp)

add_subdirectory(lib)
add_subdirectory(extlib)

target_include_directories(kittenhook PRIVATE extlib/GLFW3/include)
target_link_directories(kittenhook PRIVATE extlib/GLFW3/lib)


target_link_libraries(kittenhook PRIVATE memory-manager)
target_link_libraries(kittenhook PRIVATE glfw3)
target_link_libraries(kittenhook PRIVATE imgui)
target_link_libraries(kittenhook PRIVATE apex-sdk)
target_link_libraries(kittenhook PRIVATE uml)

find_package(OpenGL REQUIRED)
target_link_libraries(kittenhook PRIVATE ${OPENGL_LIBRARIES})
