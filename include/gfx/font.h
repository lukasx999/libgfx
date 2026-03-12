#pragma once

#include <memory>

class TextRenderer;

namespace gfx {

class Window;

class Font {
public:
    ~Font();
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;

    [[nodiscard]] int measure_char(char32_t c, int size) const;
    [[nodiscard]] int measure_text(std::string_view text, int size) const;
    [[nodiscard]] int get_height(int fontsize) const;

private:
    friend Window;
    friend TextRenderer;

    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    explicit Font(std::unique_ptr<Impl> pimpl);

};

} // namespace gfx
