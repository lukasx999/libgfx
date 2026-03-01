#pragma once

#include <memory>

class TextRenderer;

namespace gfx {

class Window;

class Font {
    friend Window;
    friend TextRenderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    explicit Font(std::unique_ptr<Impl> pimpl);

public:
    ~Font();
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;

    [[nodiscard]] int measure_char(char c, int size) const;
    [[nodiscard]] int measure_text(std::string_view text, int size) const;
    [[nodiscard]] int measure_char_total_height(char c, int size) const;

};

} // namespace gfx
