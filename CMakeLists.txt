cmake_minimum_required(VERSION 3.28)
project(modern_opengl_experiment)

set(CMAKE_CXX_STANDARD 17)

add_executable(modern_opengl_experiment main.cpp OpenGL/src/glad.c)

target_include_directories(modern_opengl_experiment PUBLIC OpenGL/include)

find_library(GLFW libglfw3 OpenGL/lib)
target_link_libraries(modern_opengl_experiment LINK_PUBLIC ${GLFW})
