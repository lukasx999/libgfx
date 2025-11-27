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
    gfx::Window& m_window;

    RectangleRenderer m_rectangle;
    CircleRenderer m_circle;
    TriangleRenderer m_triangle;
    TextureRenderer m_texture;
    LineRenderer m_line;
    TextRenderer m_text;

    glm::mat4 m_view_default = gen_view_matrix(
        m_window,
        m_window.get_width() / 2.0f,
        m_window.get_height() / 2.0f
    );
    glm::mat4 m_view_camera = m_view_default;
    glm::mat4 m_view_active = m_view_default;

    explicit Impl(gfx::Window& window)
        : m_window(window)
        , m_rectangle(m_window)
        , m_circle(m_window)
        , m_triangle(m_window)
        , m_texture(m_window)
        , m_line(m_window)
        , m_text(m_window)
    { }

    [[nodiscard]] static glm::mat4 gen_view_matrix(const Window& window, float center_x, float center_y);

};

// TODO: zoom
// TODO: provide rectangle instead of center
// TODO: replace two floats with gfx::Vec
glm::mat4 Renderer::Impl::gen_view_matrix(const Window& window, float center_x, float center_y) {
    glm::vec3 camera_position(
        center_x - window.get_width() / 2.0f,
        center_y - window.get_height() / 2.0f,
        0.0f
    );

    glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    // TODO: center rotation
    // glm::mat4 transform(1.0f);
    // transform = glm::rotate(transform, gfx::deg_to_rad(10), glm::vec3(0.0f, 0.0f, 1.0f));
    // up = transform * glm::vec4(up, 1.0);

    glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);
    return view;
}

} // namespace gfx
