#include <algorithm>
#include <print>
#include <string_view>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <gfx/font.h>
#include <gfx/error.h>
#include <gfx/rotation.h>
#include "glyph.h"
#include "font_impl.h"

namespace gfx {

Font::Font(std::unique_ptr<Impl> pimpl) : m_pimpl(std::move(pimpl)) { }

Font::~Font() = default;

int Font::measure_char(char c, int size) const {
    auto glyph = m_pimpl->load_glyph(c, size);
    return glyph.m_advance_x;
}

int Font::measure_text(std::string_view text, int size) const {
    return std::ranges::fold_left(text, 0, [&](int acc, char c) {
        return acc + measure_char(c, size);
    });
}

int Font::measure_char_total_height(char c, int size) const {
    auto glyph = m_pimpl->load_glyph(c, size);
    return size + size - glyph.m_bearing_y;
}

} // namespace gfx
