#pragma once

#include <Vec.h>

namespace gfx {

struct Rect {
    float x, y, width, height;

    [[nodiscard]] constexpr bool check_collision_rect(Rect other) const {
        bool collision_x = x+width >= other.x && other.x+other.width >= x;
        bool collision_y = y+height >= other.y && other.y+other.height >= y;
        return collision_x && collision_y;
    }

    [[nodiscard]] constexpr bool check_collision_point(gfx::Vec p) const {
        bool collision_x = p.x >= x && p.x <= x+width;
        bool collision_y = p.y >= y && p.y <= y+height;
        return collision_x && collision_y;
    }

    constexpr Rect& set_x(float new_x) {
        x = new_x;
        return *this;
    }

    constexpr Rect& set_y(float new_y) {
        y = new_y;
        return *this;
    }

    constexpr Rect& set_width(float new_width) {
        width = new_width;
        return *this;
    }

    constexpr Rect& set_height(float new_height) {
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
