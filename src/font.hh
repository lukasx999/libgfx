#pragma once

#include <font.hh>
#include "glyph.hh"

namespace gfx {

struct Font::Impl {
    [[nodiscard]] Glyph load_glyph(const Font& font, char c, unsigned int size) const;
};

} // namespace gfx
