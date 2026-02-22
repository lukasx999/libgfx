cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Gfx)
add_library(gfx STATIC)

if (EMSCRIPTEN)
    set(shader_type es)
else()
    set(shader_type gl)
endif()

execute_process(COMMAND python3 gen.py ${shader_type} WORKING_DIRECTORY shaders/)

find_package(Freetype REQUIRED)
find_package(glfw3)
find_package(OpenGL REQUIRED)

if (EMSCRIPTEN)
    target_link_options(gfx INTERFACE -sUSE_FREETYPE=1 -sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2)
else()
    target_link_libraries(gfx PRIVATE glfw ${FREETYPE_LIBRARIES} OpenGL)
endif()

target_compile_options(gfx PRIVATE -Wno-c23-extensions -Wall -Wextra -O3 -ggdb)

list(APPEND include_dirs include/ deps/glad/include/ deps/stb/ deps/glm)
include_directories(${include_dirs} ${FREETYPE_INCLUDE_DIRS})
link_directories(${FREETYPE_LIBRARY_DIRS})

add_subdirectory(src)

install(
    TARGETS gfx
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY "include/" DESTINATION "include")

# generate .cmake file for find_package()
install(FILES cmake/gfx-config.cmake DESTINATION lib/cmake/gfx)
install(TARGETS gfx EXPORT gfx)
install(EXPORT gfx DESTINATION lib/cmake/gfx)

install(FILES gfx.pc DESTINATION lib/pkgconfig)
