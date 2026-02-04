#pragma once

#include <gfx/texture.h>
#include "opengl.h"

struct gfx::Texture::Impl {
    gl::Texture m_texture;

    [[nodiscard]] static GLint gfx_format_to_opengl_format(Format format);
    [[nodiscard]] static Format opengl_format_to_gfx_format(GLint format);
    [[nodiscard]] static gl::Texture generate_texture(int width, int height, const unsigned char* data, Format format);
};
