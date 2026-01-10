#pragma once

#include <Renderer.h>
#include "shapes/RectangleRenderer.h"
#include "shapes/CircleRenderer.h"
#include "shapes/TriangleRenderer.h"
#include "shapes/TextureRenderer.h"
#include "shapes/LineRenderer.h"
#include "shapes/TextRenderer.h"

#include <glm/ext/matrix_transform.hpp>

namespace gfx {

struct Renderer::Impl {
    const gfx::Window& m_window;

    RectangleRenderer m_rectangle;
    CircleRenderer m_circle;
    TriangleRenderer m_triangle;
    TextureRenderer m_texture;
    LineRenderer m_line;
    TextRenderer m_text;

    glm::mat4 m_view_default = gen_view_matrix(
        m_window,
        { m_window.get_width() / 2.0f, m_window.get_height() / 2.0f }
    );
    glm::mat4 m_view_camera = m_view_default;
    glm::mat4 m_view_active = m_view_default;

    explicit Impl(const gfx::Window& window)
        : m_window(window)
        , m_rectangle(m_window)
        , m_circle(m_window)
        , m_triangle(m_window)
        , m_texture(m_window)
        , m_line(m_window)
        , m_text(m_window)
    { }

    [[nodiscard]] static glm::mat4 gen_view_matrix(const gfx::Window& window, gfx::Vec center);

};

} // namespace gfx
