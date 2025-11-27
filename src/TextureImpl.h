#pragma once

#include <Texture.h>

#include <glad/gl.h>

struct gfx::Texture::Impl {
    GLuint m_texture;

    [[nodiscard]] static GLint channels_to_opengl_format(int channels);
    [[nodiscard]] static int opengl_format_to_channels(GLint format);
};
