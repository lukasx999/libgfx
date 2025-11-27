#include <ft2build.h>
#include FT_FREETYPE_H

#include <Font.h>
#include <types.h>
#include "Glyph.h"
#include "FontImpl.h"

namespace gfx {

Font::Impl::Impl(FT_Library ft, const char* path) {
    if (FT_New_Face(ft, path, 0, &m_face) != 0)
        throw gfx::Error("failed to load font");
}

Font::Impl::~Impl() {
    FT_Done_Face(m_face);
}

Font::Font(std::unique_ptr<Impl> pimpl) : m_pimpl(std::move(pimpl)) { }

Font::~Font() = default;

int Font::measure_char(char c, int size) const {
    return m_pimpl->load_glyph(c, size).m_advance_x;
}

[[nodiscard]] int Font::measure_text(const char* text, int size) const {
    int result = 0;

    for (const char* c = text; *c; ++c) {
        result += measure_char(*c, size);
    }

    return result;
}

Glyph Font::Impl::load_glyph(char c, unsigned int size) const {

    if (FT_Set_Pixel_Sizes(m_face, 0, size))
        throw gfx::Error("failed to set pixel size");

    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0)
        throw gfx::Error("failed to load char");

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

} // namespace gfx
