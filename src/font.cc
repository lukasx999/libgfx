#include <stdexcept>

#include <font.hh>
#include "glyph.hh"
#include "font.hh"

namespace gfx {

Font::Font(FT_Library ft, const char* path) {
    if (FT_New_Face(ft, path, 0, &m_face) != 0) {
        throw std::runtime_error("failed to load font");
    }
}

Font::~Font() {
    FT_Done_Face(m_face);
}

int Font::measure_char(char c, int size) const {
    return m_pimpl->load_glyph(*this, c, size).m_advance_x;
}

[[nodiscard]] int Font::measure_text(const char* text, int size) const {
    int result = 0;

    for (const char* c = text; *c; ++c) {
        result += measure_char(*c, size);
    }

    return result;
}

Glyph Font::Impl::load_glyph(const Font& font, char c, unsigned int size) const {

    if (FT_Set_Pixel_Sizes(font.m_face, 0, size))
        throw std::runtime_error("failed to set pixel size");

    if (FT_Load_Char(font.m_face, c, FT_LOAD_RENDER) != 0)
        throw std::runtime_error("failed to load char");

    auto glyph = font.m_face->glyph;
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

} // namespace gfx
