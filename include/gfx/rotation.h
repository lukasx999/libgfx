#pragma once

#include <cmath>

namespace gfx {

[[nodiscard]] inline constexpr float deg_to_rad(float deg) {
    return deg * (M_PI / 180.0);
}

[[nodiscard]] inline constexpr float rad_to_deg(float rad) {
    return rad * (180.0 / M_PI);
}

class Degrees {
    const float m_value;

public:
    explicit constexpr Degrees(float value) : m_value(value) { }

    [[nodiscard]] constexpr operator float() const {
        return m_value;
    }

};

class Radians {
    const float m_value;

public:
    explicit constexpr Radians(float value) : m_value(value) { }

    [[nodiscard]] constexpr operator float() const {
        return m_value;
    }

};

class Rotation {
    const float m_radians = 0.0f;

public:
    constexpr Rotation() = default;
    constexpr Rotation(gfx::Degrees degrees) : m_radians(deg_to_rad(degrees)) { }
    constexpr Rotation(gfx::Radians radians) : m_radians(radians) { }

    [[nodiscard]] constexpr float get_degrees() const {
        return rad_to_deg(m_radians);
    }

    [[nodiscard]] constexpr float get_radians() const {
        return m_radians;
    }

};

} // namespace gfx
