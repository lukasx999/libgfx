#pragma once

#include <print>
#include <utility>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// light raii wrappers for opengl objects

struct OpenGLObject {
    GLuint id;

    OpenGLObject() = default;
    virtual ~OpenGLObject() = default;

    OpenGLObject(const OpenGLObject& other) = delete;
    OpenGLObject& operator=(const OpenGLObject& other) = delete;

    OpenGLObject(OpenGLObject&& other) : id(std::exchange(other.id, 0)) { }

    OpenGLObject& operator=(OpenGLObject&& other) {
        std::swap(id, other.id);
        return *this;
    }

    operator GLuint() const {
        return id;
    }
};

struct VertexArray : OpenGLObject {
    VertexArray() {
        glGenVertexArrays(1, &id);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }

    VertexArray(VertexArray&&) = default;
    VertexArray& operator=(VertexArray&&) = default;
};

struct Buffer : OpenGLObject {
    Buffer() {
        glGenBuffers(1, &id);
    }

    ~Buffer() {
        glDeleteBuffers(1, &id);
    }

    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;
};

struct Program : OpenGLObject {
    Program() {
        id = glCreateProgram();
    }

    ~Program() {
        glDeleteProgram(id);
    }

    Program(Program&&) = default;
    Program& operator=(Program&&) = default;
};

struct Shader : OpenGLObject {
    Shader(GLenum type) {
        id = glCreateShader(type);
    }

    ~Shader() {
        glDeleteShader(id);
    }

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;
};

[[nodiscard]] inline Shader create_shader(GLenum type, const char* src) {
    Shader shader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512] = {0};
        glGetShaderInfoLog(shader, sizeof info_log, nullptr, info_log);
        std::println(stderr, "shader compilation failed: {}", info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

[[nodiscard]] inline Program create_shader_program(const char* vertex_src, const char* fragment_src) {

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
        char info_log[512] = {0};
        glGetProgramInfoLog(program, sizeof info_log, nullptr, info_log);
        std::println(stderr, "shader program linkage failed: {}", info_log);
        exit(EXIT_FAILURE);
    }

    return program;
}
