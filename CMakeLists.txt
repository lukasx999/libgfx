cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Gfx)

execute_process(COMMAND python3 gen.py WORKING_DIRECTORY shaders/)

list(APPEND include_dirs include/ /usr/include/freetype2/ deps/glad/include/)

add_library(gfx STATIC)
target_compile_options(gfx PRIVATE -Wno-c23-extensions -Wall -Wextra -O3 -ggdb)

include_directories(deps/stb ${include_dirs})

add_subdirectory(src)
target_link_libraries(gfx PRIVATE glfw freetype)

install(
    TARGETS gfx
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY "include/" DESTINATION "include")

# generate .cmake file for find_package()
install(FILES gfx-config.cmake DESTINATION lib/cmake/gfx)
install(TARGETS gfx EXPORT gfx)
install(EXPORT gfx DESTINATION lib/cmake/gfx)

install(FILES libgfx.pc DESTINATION lib/pkgconfig)

add_executable(test test.cc)
