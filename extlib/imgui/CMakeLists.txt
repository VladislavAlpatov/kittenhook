project(imgui)
add_library(imgui STATIC source/imgui.cpp)


add_subdirectory(source)
target_include_directories(imgui PUBLIC include)
target_include_directories(imgui PRIVATE include/imgui)
target_include_directories(imgui PRIVATE ../GLFW3/include)
target_link_directories(imgui PRIVATE ../GLFW3/lib)

target_link_libraries(imgui PRIVATE glfw3)

find_package(Freetype REQUIRED)
target_link_libraries(imgui PRIVATE ${FREETYPE_LIBRARIES})
target_include_directories(imgui PRIVATE ${FREETYPE_INCLUDE_DIRS})