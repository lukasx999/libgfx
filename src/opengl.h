#pragma once

#include <utility>


// emscripten will handle opengl initialization for us, so there's no need
// to use a loader like glad
#ifdef __EMSCRIPTEN__
#define USE_GL_WEB
#else
#define USE_GL_GLAD
#endif // __EMSCRIPTEN__


#ifdef USE_GL_GLAD
#include <glad/gl.h>
#endif // USE_GL_GLAD

#ifdef USE_GL_EPOXY
#include <epoxy/gl.h>
#endif // USE_GL_EPOXY

#ifdef USE_GL_WEB
#include <webgl/webgl2.h>
#endif // USE_GL_WEB



#include <glm/glm.hpp>
#include <glm/ext.hpp>

// light raii wrappers for opengl objects

namespace gl {

struct Object {
    GLuint id;

    Object() = default;
    virtual ~Object() = default;

    Object(const Object& other) = delete;
    Object& operator=(const Object& other) = delete;

    Object(Object&& other) : id(std::exchange(other.id, 0)) { }

    Object& operator=(Object&& other) {
        std::swap(id, other.id);
        return *this;
    }

    operator GLuint() const {
        return id;
    }
};

struct VertexArray : Object {
    VertexArray() {
        glGenVertexArrays(1, &id);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }

    VertexArray(VertexArray&&) = default;
    VertexArray& operator=(VertexArray&&) = default;
};

struct Buffer : Object {
    Buffer() {
        glGenBuffers(1, &id);
    }

    ~Buffer() {
        glDeleteBuffers(1, &id);
    }

    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;
};

struct Program : Object {
    Program() {
        id = glCreateProgram();
    }

    ~Program() {
        glDeleteProgram(id);
    }

    Program(Program&&) = default;
    Program& operator=(Program&&) = default;
};

struct Shader : Object {
    Shader(GLenum type) {
        id = glCreateShader(type);
    }

    ~Shader() {
        glDeleteShader(id);
    }

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;
};

struct Texture : Object {
    Texture() {
        glGenTextures(1, &id);
    }

    ~Texture() {
        glDeleteTextures(1, &id);
    }

    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;
};

struct Framebuffer : Object {
    Framebuffer() {
        glGenFramebuffers(1, &id);
    }

    ~Framebuffer() {
        glDeleteFramebuffers(1, &id);
    }

    Framebuffer(Framebuffer&&) = default;
    Framebuffer& operator=(Framebuffer&&) = default;
};

[[nodiscard]] Shader create_shader(GLenum type, const char* src);
[[nodiscard]] Program create_shader_program(const char* vertex_src, const char* fragment_src);

} // namespace gl
