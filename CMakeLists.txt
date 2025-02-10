set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER gcc-14)

cmake_minimum_required(VERSION 3.20)

project(Chip-8 LANGUAGES C)

file(GLOB_RECURSE chip8_src "src/*.c")

add_executable(chip8 ${chip8_src})

find_package(raylib REQUIRED)

target_link_libraries(chip8 PRIVATE raylib)
target_include_directories(chip8 PRIVATE include)
target_compile_features(chip8 PRIVATE c_std_23)
target_compile_options(chip8 PRIVATE -Wall -Wextra -O2)
