#pragma once

#include <utility>
#include <gfx/surface.h>



#if 0
// emscripten will handle opengl initialization for us, so there's no need
// to use a loader like glad
#ifdef __EMSCRIPTEN__
#define USE_GL_WEB
#else
#define USE_GL_GLAD
#endif // __EMSCRIPTEN__
#endif

#define USE_SYSTEM_GL

#ifdef USE_SYSTEM_GL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif // USE_SYSTEM_GL

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

[[nodiscard]] inline glm::mat4 get_surface_projection(const gfx::Surface& surface) {
    return glm::ortho(
        0.0f,
        static_cast<float>(surface.get_width()),
        static_cast<float>(surface.get_height()),
        0.0f
    );
}

inline void set_uniform(GLuint program, const char* name, float value) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

inline void set_uniform(GLuint program, const char* name, int value) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

inline void set_uniform(GLuint program, const char* name, glm::vec2 value) {
    GLint location = glGetUniformLocation(program, name);
    glUniform2f(location, value.x, value.y);
}

inline void set_uniform(GLuint program, const char* name, glm::mat4 value) {
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

[[nodiscard]] Shader create_shader(GLenum type, const char* src);
[[nodiscard]] Program create_shader_program(const char* vertex_src, const char* fragment_src);

} // namespace gl
