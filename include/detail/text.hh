#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <window.hh>
#include <detail/detail.hh>

namespace gfx {

namespace detail {
class TextRenderer;

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

} // namespace detail

class Font {
    friend detail::TextRenderer;
    FT_Face m_face;

    Font(FT_Library ft, const char* path) {
        if (FT_New_Face(ft, path, 0, &m_face) != 0) {
            throw std::runtime_error("failed to load font");
        }
    }

public:
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;

    ~Font() {
        FT_Done_Face(m_face);
    }

    [[nodiscard]] int measure_char(char c, int size) const {
        return load_glyph(c, size).m_advance_x;
    }

    [[nodiscard]] int measure_text(const char* text, int size) const {
        int result = 0;

        for (const char* c = text; *c; ++c) {
            result += measure_char(*c, size);
        }

        return result;
    }

private:
    [[nodiscard]] detail::Glyph load_glyph(char c, unsigned int size) const {

        if (FT_Set_Pixel_Sizes(m_face, 0, size))
            throw std::runtime_error("failed to set pixel size");

        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0)
            throw std::runtime_error("failed to load char");

        auto glyph = m_face->glyph;
        int width = glyph->bitmap.width;
        int height = glyph->bitmap.rows;
        int bearing_x = glyph->bitmap_left;
        int bearing_y = glyph->bitmap_top;
        unsigned int advance_x = glyph->advance.x;
        unsigned char* buffer = glyph->bitmap.buffer;

        return {
            width,
            height,
            bearing_x,
            bearing_y,
            // advance is pixels * 64
            advance_x / 64,
            buffer
        };

    }

};

} // namespace gfx

namespace gfx::detail {

class TextRenderer {
    gfx::Window& m_window;

    FT_Library m_ft;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_uv_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_vertices {
        glm::vec2(1.0, 1.0), // top-right
        glm::vec2(0.0, 1.0), // top-left
        glm::vec2(0.0, 0.0), // bottom-left
        glm::vec2(1.0, 0.0), // bottom-right
    };

    static constexpr std::array m_uvs {
        glm::vec2(1.0, 1.0), // top-right
        glm::vec2(0.0, 1.0), // top-left
        glm::vec2(0.0, 0.0), // bottom-left
        glm::vec2(1.0, 0.0), // bottom-right
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    TextRenderer(gfx::Window& window);

    ~TextRenderer();
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    void draw(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color, glm::mat4 view);

    [[nodiscard]] gfx::Font load_font(const char* path) const {
        return { m_ft, path };
    }

private:
    void draw_char(float x, float y, const Glyph& glyph, gfx::Color color, int text_size, glm::mat4 view);

};

} // namespace gfx::detail
