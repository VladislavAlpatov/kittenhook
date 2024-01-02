cmake_minimum_required(VERSION 3.26)
project(kittenhook)

set(CMAKE_CXX_STANDARD 23)

add_executable(kittenhook source/main.cpp)

add_subdirectory(extlib)
add_subdirectory(source)

target_include_directories(kittenhook PRIVATE source)

if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(kittenhook PRIVATE -O3)
endif()

target_include_directories(kittenhook PRIVATE extlib/GLFW3/include)
target_link_directories(kittenhook PRIVATE extlib/GLFW3/lib)


target_link_libraries(kittenhook PRIVATE glfw3)
target_link_libraries(kittenhook PRIVATE imgui)
target_link_libraries(kittenhook PRIVATE apex-sdk)

find_package(OpenGL REQUIRED)
target_link_libraries(kittenhook PRIVATE ${OPENGL_LIBRARIES})

find_package(X11 REQUIRED)
target_link_libraries(kittenhook PRIVATE ${X11_LIBRARIES})

find_package(Qt6Widgets REQUIRED)
target_link_libraries(kittenhook PRIVATE Qt6::Widgets)
