#pragma once

namespace gfx {

struct Surface {
    virtual ~Surface() = default;
    [[nodiscard]] virtual int get_width() const = 0;
    [[nodiscard]] virtual int get_height() const = 0;
};

} // namespace gfx
