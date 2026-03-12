cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Example)

find_package(gfx REQUIRED)

add_executable(example main.cc)
target_link_libraries(example PRIVATE gfx)
target_compile_options(example PRIVATE -Wall -Wextra -O3 -ggdb)
