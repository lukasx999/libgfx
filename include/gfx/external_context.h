#pragma once

#include <functional>

#include <gfx/surface.h>
#include <gfx/renderer.h>

namespace gfx {

// if you already have an active GL context, you can use this class to
// render stuff into that context. the intended usage of this class is to enable
// embedding into other applications.
class ExternalContext : public gfx::Surface {
    std::function<int()> m_get_width;
    std::function<int()> m_get_height;
    gfx::Renderer m_renderer;

public:
    ExternalContext(std::function<int()> get_width, std::function<int()> get_height)
        : m_get_width(get_width)
        , m_get_height(get_height)
        , m_renderer(*this)
    { }

    ExternalContext(int fixed_width, int fixed_height)
    : ExternalContext([=] { return fixed_width; }, [=] { return fixed_height; })
    { }

    [[nodiscard]] int get_width() const override {
        return m_get_width();
    }

    [[nodiscard]] int get_height() const override {
        return m_get_height();
    }

    void draw(std::function<void(gfx::Renderer&)> draw_fn) {
        draw_fn(m_renderer);
    }

};

} // namespace gfx
