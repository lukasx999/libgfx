#pragma once

#include "util.h"

struct Glyph {
    GLuint m_texture;
    int m_bearing_x;
    int m_bearing_y;
    unsigned int m_advance_x;

    Glyph(int width, int height, int bearing_x, int bearing_y, unsigned int advance_x, const unsigned char* data)
        : m_bearing_x(bearing_x)
        , m_bearing_y(bearing_y)
        , m_advance_x(advance_x)
    {

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        glGenTextures(1, &m_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Glyph() {
        glDeleteTextures(1, &m_texture);
    }

    [[nodiscard]] int get_width() const {
        int width;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glBindTexture(GL_TEXTURE_2D, 0);
        return width;
    }

    [[nodiscard]] int get_height() const {
        int height;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        glBindTexture(GL_TEXTURE_2D, 0);
        return height;
    }

};
