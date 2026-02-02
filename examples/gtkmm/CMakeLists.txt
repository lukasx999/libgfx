cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Example)

find_package(PkgConfig)
find_package(gfx)

pkg_check_modules(GTKMM gtkmm-4.0)

add_executable(example main.cc)
target_compile_options(example PRIVATE -Wall -Wextra -O3 -ggdb)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(example ${GTKMM_LIBRARIES} gfx epoxy)
