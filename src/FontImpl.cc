#include <gfx.h>
#include "FontImpl.h"

namespace gfx {

Font::Impl::Impl(FT_Library ft, const char* path) {
    if (FT_New_Face(ft, path, 0, &m_face) != 0)
        throw gfx::Error(std::format("failed to load font: {}", path));
}

Font::Impl::~Impl() {
    FT_Done_Face(m_face);
}

Glyph Font::Impl::load_glyph(char c, unsigned int size) const {

    if (FT_Set_Pixel_Sizes(m_face, 0, size))
        throw gfx::Error("failed to load glyph: failed to set pixel size");

    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0)
        throw gfx::Error("failed to load glyph: failed to load char");

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
