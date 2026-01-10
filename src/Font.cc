#include <ft2build.h>
#include FT_FREETYPE_H

#include <Font.h>
#include <types.h>
#include "Glyph.h"
#include "FontImpl.h"

namespace gfx {

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

} // namespace gfx
