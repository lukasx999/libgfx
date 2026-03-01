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
    return m_pimpl->load_glyph(c, size).m_advance_x;
}

[[nodiscard]] int Font::measure_text(std::string_view text, int size) const {
    int result = 0;

    for (auto c : text) {
        result += measure_char(c, size);
    }

    return result;
}

} // namespace gfx
