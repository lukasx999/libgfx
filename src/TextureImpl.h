#pragma once

#include <Texture.h>
#include "opengl.h"

#include <glad/gl.h>

struct gfx::Texture::Impl {
    gl::Texture m_texture;

    [[nodiscard]] static GLint channels_to_opengl_format(int channels);
    [[nodiscard]] static int opengl_format_to_channels(GLint format);
    [[nodiscard]] static gl::Texture generate_texture(const unsigned char* data, int width, int height, int channels);
};
