#pragma once

#include <Texture.h>

#include "util.h"

struct gfx::Texture::Impl {
    GLuint m_texture;

    [[nodiscard]] static constexpr GLint channels_to_opengl_format(int channels);
    [[nodiscard]] static constexpr int opengl_format_to_channels(GLint format);
};
