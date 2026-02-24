#pragma once

#include <format>
#include <string>

#include <gfx/vec.h>

namespace gfx {

struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    [[nodiscard]] constexpr bool check_collision_rect(Rect other) const noexcept {
        bool collision_x = x+width >= other.x && other.x+other.width >= x;
        bool collision_y = y+height >= other.y && other.y+other.height >= y;
        return collision_x && collision_y;
    }

    [[nodiscard]] constexpr bool check_collision_point(gfx::Vec p) const noexcept {
        bool collision_x = p.x >= x && p.x <= x+width;
        bool collision_y = p.y >= y && p.y <= y+height;
        return collision_x && collision_y;
    }

    constexpr void scale(float value) noexcept {
        Rect r = scaled(value);
        x = r.x;
        y = r.y;
        width = r.width;
        height = r.height;
    }

    [[nodiscard]] constexpr Rect scaled(float value) const noexcept {
        float new_width = width * value;
        float new_height = height * value;

        return {
            x + (width - new_width) / 2.0f,
            y + (height - new_height) / 2.0f,
            new_width,
            new_height,
        };
    }

    [[nodiscard]] constexpr gfx::Vec xy_to_vec() const noexcept{
        return { x, y };
    }

    [[nodiscard]] constexpr gfx::Vec wh_to_vec() const noexcept{
        return { width, height };
    }

    constexpr Rect& set_x(float new_x) noexcept {
        x = new_x;
        return *this;
    }

    constexpr Rect& set_y(float new_y) noexcept {
        y = new_y;
        return *this;
    }

    constexpr Rect& set_width(float new_width) noexcept {
        width = new_width;
        return *this;
    }

    constexpr Rect& set_height(float new_height) noexcept {
        height = new_height;
        return *this;
    }

};

} // namespace gfx

template <>
struct std::formatter<gfx::Rect> : std::formatter<std::string> {
    auto format(const gfx::Rect& rect, std::format_context& ctx) const {
        auto fmt = std::format("{{ x: {}, y: {}, width: {}, height: {} }}", rect.x, rect.y, rect.width, rect.height);
        return std::formatter<std::string>::format(fmt, ctx);
    }
};
