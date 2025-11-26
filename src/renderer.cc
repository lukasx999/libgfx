#include <renderer.hh>
#include <window.hh>

#include "shapes/rectangle.hh"
#include "shapes/circle.hh"
#include "shapes/triangle.hh"
#include "shapes/texture.hh"
#include "shapes/line.hh"
#include "shapes/text.hh"
#include "window.hh"

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

    [[nodiscard]] static constexpr glm::mat4 gen_view_matrix(const Window& window, float center_x, float center_y);

};

Renderer::Renderer(Window& window)
    : m_window(window)
    , m_pimpl(std::make_unique<gfx::Renderer::Impl>(m_window))
{ }


// the pimpl pattern requires the destructor to "see" the complete
// type of the Impl structure
Renderer::~Renderer() = default;

void Renderer::with_draw_context(std::function<void()> draw_fn) {

    // calculate frame time
    double frame_start = glfwGetTime();
    m_frame_time = frame_start - m_last_frame;
    m_last_frame = frame_start;

    draw_fn();

    glfwSwapBuffers(m_window.m_pimpl->m_window);
    glfwPollEvents();

    // sleep for the rest of the frame to keep the desired framerate steady
    if (m_desired_fps == 0.0) return;
    double frame_end = glfwGetTime() - frame_start;

    struct timespec ts{};
    ts.tv_nsec = (1.0 / m_desired_fps - frame_end) * 1e9,
        nanosleep(&ts, nullptr);
}

void Renderer::with_camera(std::function<void()> draw_fn) {
    m_pimpl->m_view_active = m_pimpl->m_view_camera;
    draw_fn();
    m_pimpl-> m_view_active = m_pimpl->m_view_default;
}

void Renderer::set_camera(float center_x, float center_y) {
    m_pimpl->m_view_camera = m_pimpl->gen_view_matrix(m_window, center_x, center_y);
}

void Renderer::draw_rectangle(float x, float y, float width, float height, const gfx::IRotation& rotation, gfx::Color color) {
    m_pimpl->m_rectangle.draw(x, y, width, height, rotation, color, m_pimpl->m_view_active);
}

void Renderer::draw_texture(float x, float y, float width, float height, const gfx::IRotation& rotation, const gfx::Texture& texture) {
    m_pimpl->m_texture.draw(x, y, width, height, rotation, texture, m_pimpl->m_view_active);
}

void Renderer::draw_texture_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, const gfx::IRotation& rotation, const gfx::Texture& texture) {
    m_pimpl->m_texture.draw_sub(dest_x, dest_y, dest_width, dest_height, src_x, src_y, src_width, src_height, rotation, texture, m_pimpl->m_view_active);
}

void Renderer::draw_circle(float x, float y, float radius, gfx::Color color) {
    m_pimpl->m_circle.draw(x, y, radius, color, m_pimpl->m_view_active);
}

void Renderer::draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color) {
    m_pimpl->m_triangle.draw(x0, y0, x1, y1, x2, y2, color, m_pimpl->m_view_active);
}

void Renderer::draw_line(float x0, float y0, float x1, float y1, gfx::Color color) {
    m_pimpl->m_line.draw(x0, y0, x1, y1, color, m_pimpl->m_view_active);
}

void Renderer::draw_text(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
    m_pimpl->m_text.draw(x, y, text_size, text, font, color, m_pimpl->m_view_active);
}

gfx::Font Renderer::load_font(const char* path) const {
    return m_pimpl->m_text.load_font(path);
}

void Renderer::clear_background(Color color) {
    auto normalized = color.normalized();
    glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

// TODO: zoom
// TODO: provide rectangle instead of center
// TODO: replace two floats with gfx::Vec
constexpr glm::mat4 gfx::Renderer::Impl::gen_view_matrix(const Window& window, float center_x, float center_y) {
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
