#pragma once

#include <gfx/texture.h>
#include "opengl.h"

struct Glyph {
    const int m_bearing_x;
    const int m_bearing_y;
    const unsigned int m_advance_x;
    gfx::Texture m_texture;

    Glyph(int width, int height, int bearing_x, int bearing_y, unsigned int advance_x, const unsigned char* data)
        : m_bearing_x(bearing_x)
        , m_bearing_y(bearing_y)
        , m_advance_x(advance_x)
        , m_texture(width, height, gfx::Texture::Format::R, data)
    {

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    [[nodiscard]] int get_width() const {
        return m_texture.get_width();
    }

    [[nodiscard]] int get_height() const {
        return m_texture.get_height();
    }

};
