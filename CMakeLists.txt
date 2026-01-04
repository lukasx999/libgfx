cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Gfx)

list(APPEND include_dirs include/ /usr/include/freetype2/ deps/glad/include)

# add_custom_command(OUTPUT src/shaders.hh COMMAND python gen.py WORKING_DIRECTORY shaders COMMENT "packing shaders" CODEGEN)
# add_custom_command(TARGET gfx PRE_BUILD COMMAND python gen.py WORKING_DIRECTORY shaders COMMENT "packing shaders")

add_library(gfx)
target_compile_options(gfx PRIVATE -Wno-c23-extensions -Wall -Wextra -O3 -ggdb)
include_directories(deps/stb ${include_dirs})
target_include_directories(gfx PUBLIC ${include_dirs})
add_subdirectory(src)
target_link_libraries(gfx PRIVATE glfw freetype)
