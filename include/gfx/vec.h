#pragma once

#include <cmath>
#include <format>
#include <string>

#include <gfx/rotation.h>

namespace gfx {

struct Vec {
    float x = 0.0f;
    float y = 0.0f;

    constexpr void normalize() noexcept {
        auto v = normalized();
        x = v.x;
        y = v.y;
    }

    [[nodiscard]] constexpr Vec normalized() const noexcept {
        float len = length();
        return {
            x / len,
            y / len,
        };
    }

    [[nodiscard]] constexpr float length() const noexcept {
        return std::sqrt(x*x + y*y);
    }

    [[nodiscard]] constexpr float dot(const Vec& other) const noexcept {
        return *this * other;
    }

    [[nodiscard]] constexpr float angle(const Vec& other) const noexcept {
        return std::acos((*this * other) / (length() * other.length()));
    }

    [[nodiscard]] constexpr float distance(const Vec& other) const noexcept {
        auto diff = other - *this;
        return std::sqrt(diff.x*diff.x + diff.y*diff.y);
    }

    [[nodiscard]] constexpr Vec rotated(gfx::Rotation angle) const noexcept {
        Vec result;

        float cos_ = std::cosf(angle.get_radians());
        float sin_ = std::sinf(angle.get_radians());

        result.x = cos_ * x - sin_ * y;
        result.y = sin_ * x + cos_ * y;

        return result;
    }

    constexpr void rotate(gfx::Rotation angle) noexcept {
        Vec v = rotated(angle);
        x = v.x;
        y = v.y;
    }

    constexpr bool operator<=>(const Vec& other) const noexcept = default;

    constexpr Vec operator+(const Vec& other) const noexcept {
        return {
            x + other.x,
            y + other.y,
        };
    }

    constexpr Vec& operator+=(const Vec& other) noexcept {
        x += other.x;
        y += other.y;

        return *this;
    }

    constexpr float operator*(const Vec& other) const noexcept {
        return x * other.x + y * other.y;
    }

    constexpr Vec operator*(float value) const noexcept {
        return {
            x * value,
            y * value,
        };
    }

    constexpr Vec operator-(const Vec& other) const noexcept {
        return {
            x - other.x,
            y - other.y,
        };
    }

    constexpr Vec& set_x(float new_x) noexcept {
        x = new_x;
        return *this;
    }

    constexpr Vec& set_y(float new_y) noexcept{
        y = new_y;
        return *this;
    }

};

constexpr Vec operator*(float value, const gfx::Vec& v) {
    return v * value;
}

} // namespace gfx

template <>
struct std::formatter<gfx::Vec> : std::formatter<std::string> {
    auto format(const gfx::Vec& vec, std::format_context& ctx) const {
        auto fmt = std::format("{{ x: {}, y: {} }}", vec.x, vec.y);
        return std::formatter<std::string>::format(fmt, ctx);
    }
};
