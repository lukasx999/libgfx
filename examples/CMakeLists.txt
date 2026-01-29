cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Examples)

find_package(gfx)

add_executable(shapes shapes.cc)
target_compile_options(shapes PRIVATE -Wall -Wextra -O3 -ggdb)
target_link_libraries(shapes PRIVATE gfx)

add_executable(offscreen_rendering offscreen_rendering.cc)
target_compile_options(offscreen_rendering PRIVATE -Wall -Wextra -O3 -ggdb)
target_link_libraries(offscreen_rendering PRIVATE gfx)

add_executable(clock clock.cc)
target_compile_options(clock PRIVATE -Wall -Wextra -O3 -ggdb)
target_link_libraries(clock PRIVATE gfx)

add_executable(animation animation.cc)
target_compile_options(animation PRIVATE -Wall -Wextra -O3 -ggdb)
target_link_libraries(animation PRIVATE gfx)
