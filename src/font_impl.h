#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <font.h>
#include "glyph.h"

namespace gfx {

struct Font::Impl {
    FT_Face m_face;
    [[nodiscard]] Glyph load_glyph(char c, unsigned int size) const;
    Impl(FT_Library ft, const char* path);
    ~Impl();
};

} // namespace gfx
