#pragma once

#include <cstdint>
#include <string>
#include <format>

namespace gfx {

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    Color() = default;

    Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    { }

    Color(uint32_t color)
        : r(color >> 8*3 & 0xff)
        , g(color >> 8*2 & 0xff)
        , b(color >> 8*1 & 0xff)
        , a(color >> 8*0 & 0xff)
    { }

    // these methods allow you to partially modify colors, within one expression.

    Color& set_red(uint8_t value) {
        r = value;
        return *this;
    }

    Color& set_green(uint8_t value) {
        g = value;
        return *this;
    }

    Color& set_blue(uint8_t value) {
        b = value;
        return *this;
    }

    Color& set_alpha(uint8_t value) {
        a = value;
        return *this;
    }

    [[nodiscard]] constexpr auto normalized() const {
        struct NormalizedColor {
            float r, g, b, a;
        };

        return NormalizedColor {
            r / 255.0f,
            g / 255.0f,
            b / 255.0f,
            a / 255.0f
        };
    }

    [[nodiscard]] static constexpr Color black() {
        return 0x000000ff;
    }

    [[nodiscard]] static constexpr Color orange() {
        return 0xffa500ff;
    }

    [[nodiscard]] static constexpr Color white() {
        return 0xffffffff;
    }

    [[nodiscard]] static constexpr Color grey() {
        return 0x808080ff;
    }

    [[nodiscard]] static constexpr Color gray() {
        return grey();
    }

    [[nodiscard]] static constexpr Color red() {
        return 0xff0000ff;
    }

    [[nodiscard]] static constexpr Color green() {
        return 0x00ff00ff;
    }

    [[nodiscard]] static constexpr Color blue() {
        return 0x0000ffff;
    }

};

} // namespace gfx

template <>
struct std::formatter<gfx::Color> : std::formatter<std::string> {
    auto format(const gfx::Color& color, std::format_context& ctx) const {
        auto fmt = std::format(
            "{{ r: {}, g: {}, b: {}, a: {} }}",
            color.r,
            color.g,
            color.b,
            color.a
        );
        return std::formatter<std::string>::format(fmt, ctx);
    }
};
