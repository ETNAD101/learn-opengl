cmake_minimum_required(VERSION 3.26)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 3.3 REQUIRED)

add_library(
        glad
        include/glad/glad.h
        src/glad.c
)

add_library(
        stb_implementation
        src/stb_image_implementation.cpp
)

include_directories(include)

add_library(
        shader
        include/shader.h
        src/shader.cpp
)

add_library(
        camera
        include/camera.h
        src/camera.cpp
)

add_executable(
        learn_opengl
        src/main.cpp
)

target_link_libraries(
        learn_opengl
        glad
        glfw
        stb_implementation
        shader
        camera
)

