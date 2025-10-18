#pragma once

#include <cmath>

#include <types.hh>

namespace gfx {

struct Vec {
    float x = 0.0f;
    float y = 0.0f;

    constexpr float distance(const Vec& other) const {
        auto diff = other - *this;
        return std::sqrt(diff.x*diff.x + diff.y*diff.y);
    }

    [[nodiscard]] constexpr Vec rotated(const gfx::IRotation& angle) const {
        Vec result;

        float cos_ = std::cosf(angle.get_radians());
        float sin_ = std::sinf(angle.get_radians());

        result.x = cos_ * x - sin_ * y;
        result.y = sin_ * x + cos_ * y;

        return result;
    }

    constexpr void rotate(const gfx::IRotation& angle) {
        Vec v = rotated(angle);
        x = v.x;
        y = v.y;
    }

    constexpr bool operator<=>(const Vec& other) const = default;

    constexpr Vec operator+(const Vec& other) const {
        return {
            x + other.x,
            y + other.y,
        };
    }

    constexpr Vec operator*(const Vec& other) const {
        return {
            x * other.x,
            y * other.y,
        };
    }

    constexpr Vec operator-(const Vec& other) const {
        return {
            x - other.x,
            y - other.y,
        };
    }

    constexpr Vec operator/(const Vec& other) const {
        return {
            x / other.x,
            y / other.y,
        };
    }

};

} // namespace gfx
