#pragma once

#include <gfx/vec.h>
#include <gfx/rect.h>

namespace gfx {

struct Surface {
    virtual ~Surface() = default;

    [[nodiscard]] virtual int get_width() const = 0;
    [[nodiscard]] virtual int get_height() const = 0;

    [[nodiscard]] gfx::Vec get_center() const {
        return {
            get_width() / 2.0f,
            get_height() / 2.0f,
        };
    }

    [[nodiscard]] gfx::Rect get_as_rect() const {
        return {
            0.0f,
            0.0f,
            static_cast<float>(get_width()),
            static_cast<float>(get_height()),
        };
    }

};

} // namespace gfx
