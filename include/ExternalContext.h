#pragma once

#include <functional>

#include "Surface.h"
#include "Renderer.h"

namespace gfx {

// if you already have an active opengl context, you can use this class to
// render stuff into that context. the intended usage of this class is to enable
// embedding into other applications.
class ExternalContext : public gfx::Surface {
    const int m_width;
    const int m_height;
    gfx::Renderer m_renderer;

public:
    ExternalContext(int width, int height)
        : m_width(width)
        , m_height(height)
        , m_renderer(*this)
    { }

    [[nodiscard]] int get_width() const override {
        return m_width;
    }

    [[nodiscard]] int get_height() const override {
        return m_height;
    }

    void draw(std::function<void(gfx::Renderer&)> draw_fn) {
        draw_fn(m_renderer);
    }

};

} // namespace gfx
