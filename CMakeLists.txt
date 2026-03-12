cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Gfx)
add_library(gfx STATIC)

find_package(Freetype REQUIRED)
find_package(glfw3)
find_package(OpenGL REQUIRED)


if (EMSCRIPTEN)
    set(shader_type es)
    target_link_options(gfx INTERFACE -sUSE_FREETYPE=1 -sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2)

else()
    set(shader_type gl)
    target_link_libraries(gfx PRIVATE glfw ${FREETYPE_LIBRARIES} OpenGL wayland-client)

endif()




# SHADER CODEGEN

file(COPY shaders/gles3 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY shaders/opengl4.5 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
  OUTPUT shaders.h
  COMMAND python ${PROJECT_SOURCE_DIR}/shaders/generate.py ${shader_type}
  DEPENDS shaders
)

add_custom_target(shaders DEPENDS shaders.h)
add_dependencies(gfx shaders)
target_include_directories(gfx PRIVATE ${PROJECT_BINARY_DIR})




# WAYLAND PROTOCOL CODEGEN

add_custom_command(
  OUTPUT xdg-shell.c
  COMMAND wayland-scanner private-code ${PROJECT_SOURCE_DIR}/protocols/xdg-shell.xml xdg-shell.c
  DEPENDS protocols
)
target_sources(gfx PRIVATE xdg-shell.c)

add_custom_command(
  OUTPUT xdg-shell.h
  COMMAND wayland-scanner client-header ${PROJECT_SOURCE_DIR}/protocols/xdg-shell.xml xdg-shell.h
  DEPENDS protocols
)
add_custom_target(xdg-shell DEPENDS xdg-shell.h)
add_dependencies(gfx xdg-shell)

add_custom_command(
  OUTPUT wlr-layer-shell-unstable-v1.h
  COMMAND wayland-scanner client-header ${PROJECT_SOURCE_DIR}/protocols/wlr-layer-shell-unstable-v1.xml wlr-layer-shell-unstable-v1.h
  DEPENDS protocols
)
add_custom_target(wlr-layer-shell DEPENDS wlr-layer-shell-unstable-v1.h)
add_dependencies(gfx wlr-layer-shell)

add_custom_command(
  OUTPUT wlr-layer-shell-unstable-v1.c
  COMMAND wayland-scanner private-code ${PROJECT_SOURCE_DIR}/protocols/wlr-layer-shell-unstable-v1.xml wlr-layer-shell-unstable-v1.c
  DEPENDS protocols
)
target_sources(gfx PRIVATE wlr-layer-shell-unstable-v1.c)

target_link_libraries(gfx PRIVATE EGL wayland-egl)






target_compile_options(gfx PRIVATE -Wno-c23-extensions -Wall -Wextra -O3 -ggdb)

# TODO: remove some unneeded includes
list(APPEND include_dirs include/ deps/glad/include/ deps/stb/ deps/glm)
target_include_directories(gfx PRIVATE ${include_dirs} ${FREETYPE_INCLUDE_DIRS})
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
