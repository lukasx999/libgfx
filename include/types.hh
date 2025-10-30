#pragma once

#include <cmath>
#include <cstdint>

namespace gfx {

struct Rect {
    float x, y, width, height;

    [[nodiscard]] constexpr bool check_collision_rects(Rect other) const {
        bool collision_x = x+width >= other.x && other.x+other.width >= x;
        bool collision_y = y+height >= other.y && other.y+other.height >= y;
        return collision_x && collision_y;
    }

};

[[nodiscard]] inline constexpr float deg_to_rad(float deg) {
    return deg * (M_PI / 180.0);
}

[[nodiscard]] inline constexpr float rad_to_deg(float rad) {
    return rad * (180.0 / M_PI);
}

class IRotation {
public:
    [[nodiscard]] virtual float get_radians() const = 0;
    [[nodiscard]] virtual float get_degrees() const = 0;
    virtual ~IRotation() = default;
};

class Radians : public IRotation {
    float m_radians;

public:
    explicit constexpr Radians(float radians) : m_radians(radians) { }

    [[nodiscard]] constexpr float get_radians() const override {
        return m_radians;
    }

    [[nodiscard]] constexpr float get_degrees() const override {
        return rad_to_deg(m_radians);
    }

};

class Degrees : public IRotation {
    float m_degrees;

public:
    explicit constexpr Degrees(float degrees) : m_degrees(degrees) { }

    [[nodiscard]] constexpr float get_radians() const override {
        return deg_to_rad(m_degrees);
    }

    [[nodiscard]] constexpr float get_degrees() const override {
        return m_degrees;
    }

};

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

class KeyState {
    friend class Window;
    int m_glfw_state;
    explicit KeyState(int glfw_state) : m_glfw_state(glfw_state) { }

public:
    [[nodiscard]] bool pressed() const {
        return m_glfw_state == GLFW_PRESS;
    }

    [[nodiscard]] bool released() const {
        return m_glfw_state == GLFW_RELEASE;
    }

    [[nodiscard]] bool repeated() const {
        return m_glfw_state == GLFW_REPEAT;
}

};

enum class MouseButton {
    Left, Right, Middle,
};

enum class Key {
    Escape,
    Space,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
};

} // namespace gfx

inline consteval gfx::Degrees operator""_deg(unsigned long long value) {
    return gfx::Degrees(value);
}

inline consteval gfx::Radians operator""_rad(unsigned long long value) {
    return gfx::Radians(value);
}
