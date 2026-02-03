#include <gfx/renderer.h>
#include <gfx/window.h>
#include "renderer_impl.h"
#include "texture_impl.h"

namespace gfx {

Renderer::Renderer(const gfx::Surface& surface)
    : m_surface(surface)
    , m_pimpl(std::make_unique<gfx::Renderer::Impl>(m_surface))
{ }

// the pimpl pattern requires the destructor to "see" the complete
// type of the Impl structure
Renderer::~Renderer() = default;

gfx::Texture Renderer::to_texture(DrawFn draw_fn) {

    gfx::Texture texture(m_surface.get_width(), m_surface.get_height(), 3);

    gl::Framebuffer framebuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_pimpl->m_texture, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    draw_fn();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return texture;
}

void Renderer::with_camera(DrawFn draw_fn) {
    m_pimpl->m_view_active = m_pimpl->m_view_camera;
    draw_fn();
    m_pimpl-> m_view_active = m_pimpl->m_view_default;
}

void Renderer::set_camera(gfx::Vec center, gfx::Rotation rotation, float scale) {
    m_pimpl->m_view_camera = m_pimpl->gen_view_matrix(m_surface, center, rotation, scale);
}

void Renderer::draw_rectangle(gfx::Rect dest, gfx::Rotation rotation, gfx::Color color) {
    m_pimpl->m_rectangle.draw(dest, rotation, color, m_pimpl->m_view_active);
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

void Renderer::draw_line(gfx::Vec a, gfx::Vec b, gfx::Color color) {
    m_pimpl->m_line.draw(a, b, color, m_pimpl->m_view_active); }

void Renderer::draw_text(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
    m_pimpl->m_text.draw(pos, fontsize, text, font, color, m_pimpl->m_view_active);
}

void Renderer::clear_background(Color color) {
    auto normalized = color.normalized();
    glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace gfx
