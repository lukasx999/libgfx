#pragma once

#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

class TextRenderer;

namespace gfx {

class Font {
    friend TextRenderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    FT_Face m_face;

    Font(FT_Library ft, const char* path);

public:
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;
    ~Font();

    [[nodiscard]] int measure_char(char c, int size) const;
    [[nodiscard]] int measure_text(const char* text, int size) const;

};

} // namespace gfx
