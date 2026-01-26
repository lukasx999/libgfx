#pragma once

#include <cmath>
#include <string>
#include <stdexcept>

namespace gfx {

struct Error : std::runtime_error {
    explicit Error(const char* msg) : std::runtime_error(msg) { }
    explicit Error(const std::string& msg) : std::runtime_error(msg) { }
};

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

inline consteval gfx::Degrees operator""_deg(unsigned long long value) {
    return gfx::Degrees(value);
}

inline consteval gfx::Radians operator""_rad(unsigned long long value) {
    return gfx::Radians(value);
}
