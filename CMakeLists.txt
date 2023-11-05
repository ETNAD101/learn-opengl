cmake_minimum_required(VERSION 3.26)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 23)
set(IMGUI_INCLUDE "include/ImGui")
set(IMGUI_SRC "src/ImGui")

find_package(glfw3 3.3 REQUIRED)

include_directories(include)

add_library(
        glad
        include/glad/glad.h
        src/glad.c
)

add_library(
        stb_implementation
        src/stb_image_implementation.cpp
)

add_library(
        ImGui
        ${IMGUI_INCLUDE}/imconfig.h
        ${IMGUI_INCLUDE}/imgui.h
        ${IMGUI_INCLUDE}/imgui_impl_glfw.h
        ${IMGUI_INCLUDE}/imgui_impl_opengl3.h
        ${IMGUI_INCLUDE}/imgui_impl_opengl3_loader.h
        ${IMGUI_INCLUDE}/imgui_internal.h
        ${IMGUI_INCLUDE}/imstb_rectpack.h
        ${IMGUI_INCLUDE}/imstb_textedit.h
        ${IMGUI_INCLUDE}/imstb_truetype.h
        ${IMGUI_SRC}/imgui.cpp
        ${IMGUI_SRC}/imgui_demo.cpp
        ${IMGUI_SRC}/imgui_draw.cpp
        ${IMGUI_SRC}/imgui_impl_glfw.cpp
        ${IMGUI_SRC}/imgui_impl_opengl3.cpp
        ${IMGUI_SRC}/imgui_tables.cpp
        ${IMGUI_SRC}/imgui_widgets.cpp
)

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
        ImGui
        glad
        glfw
        stb_implementation
        shader
        camera
)

