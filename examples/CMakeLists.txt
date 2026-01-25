cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Shapes)
add_executable(shapes shapes.cc)
