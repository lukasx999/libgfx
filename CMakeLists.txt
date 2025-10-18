cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Gfx)

add_library(gfx)
target_compile_options(gfx PRIVATE "-Wno-c23-extensions" "-Wall" "-Wextra" "-O3")
include_directories(deps/stb)
target_include_directories(gfx PUBLIC /usr/include/freetype2/)
target_include_directories(gfx PUBLIC deps/glad/include)
target_include_directories(gfx PUBLIC include)
add_subdirectory(src)
target_link_libraries(gfx PRIVATE glfw freetype)

# add_executable(tests test.cc)
