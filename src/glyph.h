#pragma once

#include <gfx/texture.h>
#include "opengl.h"

struct Glyph {
    const int m_bearing_x;
    const int m_bearing_y;
    const unsigned int m_advance_x;
    const gfx::Texture m_texture;

    Glyph(int width, int height, int bearing_x, int bearing_y, unsigned int advance_x, const unsigned char* data)
        : m_bearing_x(bearing_x)
        , m_bearing_y(bearing_y)
        , m_advance_x(advance_x)
        , m_texture(width, height, gfx::Texture::Format::R, data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    }

};
