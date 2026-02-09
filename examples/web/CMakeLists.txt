cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Example)

find_package(gfx REQUIRED)

add_executable(index main.cc)
file(COPY index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(index PRIVATE -sNO_DISABLE_EXCEPTION_CATCHING -Wall -Wextra -ggdb)
target_link_libraries(index PRIVATE gfx)
