#pragma once

#include <cmath>

namespace gfx {

struct Rect {
    float x, y, width, height;

    [[nodiscard]] constexpr bool check_collision(Rect other) const {
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

} // namespace gfx

inline consteval gfx::Degrees operator""_deg(unsigned long long value) {
    return gfx::Degrees(value);
}

inline consteval gfx::Radians operator""_rad(unsigned long long value) {
    return gfx::Radians(value);
}
