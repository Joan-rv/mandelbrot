cmake_minimum_required(VERSION 3.10)
project(mandelbrot)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mandelbrot src/main.cpp)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)

target_link_libraries(mandelbrot raylib)

if(MSVC)
    target_compile_options(mandelbrot PRIVATE /W4)
else()
    target_compile_options(mandelbrot PRIVATE -Wall -Wextra -Wpedantic -Wno-missing-field-initializers)
endif()
