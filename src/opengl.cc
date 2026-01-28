#include <stdexcept>
#include <format>

#include "opengl.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

namespace gl {

Shader create_shader(GLenum type, const char* src) {
    Shader shader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::array<char, 512> info_log;
        glGetShaderInfoLog(shader, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error(std::format("shader compilation failed: {}", info_log));
    }

    return shader;
}

Program create_shader_program(const char* vertex_src, const char* fragment_src) {

    Shader vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_src);
    Shader fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_src);
    Program program;

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glUseProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        std::array<char, 512> info_log;
        glGetProgramInfoLog(program, info_log.size(), nullptr, info_log.data());
        throw std::runtime_error(std::format("shader program linkage failed: {}", info_log));
    }

    return program;
}

} // namespace gl
