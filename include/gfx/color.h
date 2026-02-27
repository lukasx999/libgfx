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

    constexpr Color() = default;

    constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : r(red)
        , g(green)
        , b(blue)
        , a(alpha)
    { }

    constexpr Color(uint8_t red, uint8_t green, uint8_t blue)
    : Color(red, green, blue, 0xff)
    { }

    constexpr Color(uint8_t value, uint8_t alpha)
    : Color(value, value, value, alpha)
    { }

    constexpr Color(uint32_t color)
        : r(color >> 8*3 & 0xff)
        , g(color >> 8*2 & 0xff)
        , b(color >> 8*1 & 0xff)
        , a(color >> 8*0 & 0xff)
    { }

    constexpr bool operator<=>(const Color& other) const = default;

    [[nodiscard]] constexpr Color operator*(float value) const {
        return {
            static_cast<uint8_t>(r * value),
            static_cast<uint8_t>(g * value),
            static_cast<uint8_t>(b * value),
            static_cast<uint8_t>(a * value),
        };
    }

    // these methods allow you to partially modify colors, within one expression.

    constexpr Color& set_red(uint8_t value) {
        r = value;
        return *this;
    }

    constexpr Color& set_green(uint8_t value) {
        g = value;
        return *this;
    }

    constexpr Color& set_blue(uint8_t value) {
        b = value;
        return *this;
    }

    constexpr Color& set_alpha(uint8_t value) {
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

    [[nodiscard]] static constexpr Color transparent() {
        return {};
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

    [[nodiscard]] static constexpr Color lightblue() {
        return 0x90d5ffff;
    }

};

[[nodiscard]] constexpr Color operator*(float value, const Color& color) {
    return color * value;
}

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
