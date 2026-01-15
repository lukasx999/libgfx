#include <Renderer.h>
#include <Window.h>
#include "WindowImpl.h"
#include "RendererImpl.h"

bool library_has_been_initialized = false;

namespace gfx {

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
    ts.tv_nsec = (1.0 / m_desired_fps - frame_end) * 1e9;
    nanosleep(&ts, nullptr);
}

void Renderer::with_camera(std::function<void()> draw_fn) {
    m_pimpl->m_view_active = m_pimpl->m_view_camera;
    draw_fn();
    m_pimpl-> m_view_active = m_pimpl->m_view_default;
}

void Renderer::set_camera(float center_x, float center_y) {
    m_pimpl->m_view_camera = m_pimpl->gen_view_matrix(m_window, { center_x, center_y });
}

void Renderer::draw_rectangle(float x, float y, float width, float height, gfx::Rotation rotation, gfx::Color color) {
    m_pimpl->m_rectangle.draw(x, y, width, height, rotation, color, m_pimpl->m_view_active);
}

void Renderer::draw_texture(float x, float y, float width, float height, gfx::Rotation rotation, const gfx::Texture& texture) {
    m_pimpl->m_texture.draw(x, y, width, height, rotation, texture, m_pimpl->m_view_active);
}

void Renderer::draw_texture_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, gfx::Rotation rotation, const gfx::Texture& texture) {
    m_pimpl->m_texture.draw_sub(dest_x, dest_y, dest_width, dest_height, src_x, src_y, src_width, src_height, rotation, texture, m_pimpl->m_view_active);
}

void Renderer::draw_circle(gfx::Vec center, float radius, gfx::Color color) {
    m_pimpl->m_circle.draw(center, radius, color, m_pimpl->m_view_active);
}

void Renderer::draw_triangle(gfx::Vec a, gfx::Vec b, gfx::Vec c, gfx::Color color) {
    m_pimpl->m_triangle.draw(a, b, c, color, m_pimpl->m_view_active);
}

void Renderer::draw_line(gfx::Vec start, gfx::Vec end, gfx::Color color) {
    m_pimpl->m_line.draw(start, end, color, m_pimpl->m_view_active);
}

void Renderer::draw_text(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
    m_pimpl->m_text.draw(x, y, text_size, text, font, color, m_pimpl->m_view_active);
}

void Renderer::clear_background(Color color) {
    auto normalized = color.normalized();
    glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace gfx
