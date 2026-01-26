#pragma once

#include <gfx/renderer.h>
#include "shapes/rectangle_renderer.h"
#include "shapes/circle_renderer.h"
#include "shapes/triangle_renderer.h"
#include "shapes/texture_renderer.h"
#include "shapes/line_renderer.h"
#include "shapes/text_renderer.h"

#include <glm/ext/matrix_transform.hpp>

namespace gfx {

struct Renderer::Impl {
    const gfx::Surface& m_surface;

    RectangleRenderer m_rectangle;
    CircleRenderer m_circle;
    TriangleRenderer m_triangle;
    TextureRenderer m_texture;
    LineRenderer m_line;
    TextRenderer m_text;

    glm::mat4 m_view_default = gen_view_matrix(
        m_surface,
        { m_surface.get_width() / 2.0f, m_surface.get_height() / 2.0f }
    );
    glm::mat4 m_view_camera = m_view_default;
    glm::mat4 m_view_active = m_view_default;

    explicit Impl(const gfx::Surface& surface)
        : m_surface(surface)
        , m_rectangle(m_surface)
        , m_circle(m_surface)
        , m_triangle(m_surface)
        , m_texture(m_surface)
        , m_line(m_surface)
        , m_text(m_surface)
    { }

    [[nodiscard]] static glm::mat4 gen_view_matrix(const gfx::Surface& surface, gfx::Vec center);

};

} // namespace gfx
