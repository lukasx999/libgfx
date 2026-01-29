#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <gfx/font.h>
#include "glyph.h"

namespace gfx {

struct Font::Impl {
    FT_Face m_face;

    Impl(FT_Library ft, const char* path);
    ~Impl();
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;

    [[nodiscard]] Glyph load_glyph(char c, unsigned int size) const;

};

} // namespace gfx
