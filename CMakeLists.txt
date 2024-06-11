cmake_minimum_required(VERSION 3.28)
project(modern_opengl_experiment)

set(CMAKE_CXX_STANDARD 17)

add_executable(modern_opengl_experiment main.cpp OpenGL/src/glad.c)

target_include_directories(modern_opengl_experiment PUBLIC OpenGL/include)

if (MINGW)
    message("Windows using the MinGW compiler detected.")
    find_library(GLFW libglfw3 OpenGL/lib)
    target_link_libraries(modern_opengl_experiment LINK_PUBLIC ${GLFW})
endif (MINGW)

if (UNIX)
    message("Linux Detected.")
    find_package(glfw3 3.4 REQUIRED)
    target_link_libraries(modern_opengl_experiment glfw)
endif (UNIX)

