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

    gfx::Texture texture(m_surface.get_width(), m_surface.get_height(), gfx::Texture::Format::RGB);

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

void Renderer::draw_rectangle(gfx::Rect rect, gfx::Color color, gfx::Rotation rotation) {
    m_pimpl->m_rectangle.draw(rect, color, rotation, m_pimpl->m_view_active);
}

void Renderer::draw_rectangle_rounded(gfx::Rect rect, gfx::Color color, float radius) {

    draw_circle(rect.x+radius, rect.y+radius, radius, color); // top left
    draw_circle(rect.x+rect.width-radius, rect.y+radius, radius, color); // top right
    draw_circle(rect.x+radius, rect.y+rect.height-radius, radius, color); // bottom left
    draw_circle(rect.x+rect.width-radius, rect.y+rect.height-radius, radius, color); // bottom right

    draw_rectangle(gfx::Rect(rect)
                   .set_y(rect.y+radius)
                   .set_height(rect.height-radius*2), color);

    draw_rectangle(gfx::Rect(rect)
                   .set_x(rect.x+radius)
                   .set_width(rect.width-radius*2), color);
}

void Renderer::draw_texture(gfx::Rect rect, const gfx::Texture& texture, gfx::Rotation rotation) {
    m_pimpl->m_texture.draw(rect, texture, rotation, m_pimpl->m_view_active);
}

void Renderer::draw_texture_sub(gfx::Rect dest, gfx::Rect src, const gfx::Texture& texture, gfx::Rotation rotation) {
    m_pimpl->m_texture.draw_sub(dest, src, texture, rotation, m_pimpl->m_view_active);
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

// draw a thick line by drawing a vertical rectangle between the two points, and
// rotating it to match the slope of the line
void Renderer::draw_line_thick(gfx::Vec start, gfx::Vec end, float thickness, gfx::Color color) {
    float width = end.x - start.x;
    float height = end.y - start.y;
    float rotation = -std::atan((width/2.0f) / (height/2.0f));

    float length = (end - start).length();
    float x = start.x + width/2.0f - thickness/2.0f;
    float y = start.y + height/2.0f - length/2.0f;
    draw_rectangle(x, y, thickness, length, color, gfx::Radians(rotation));
}

void Renderer::draw_text(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color, gfx::Rotation rotation) {
    m_pimpl->m_text.draw(pos, fontsize, text, font, color, rotation, m_pimpl->m_view_active);
}

void Renderer::draw_quadratic_bezier_curve(gfx::Vec p1, gfx::Vec p2, gfx::Vec ctl, float thickness, gfx::Color color) {
    gfx::Vec last = p1;

    for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
        auto a = gfx::lerp(p1, ctl, t);
        auto b = gfx::lerp(ctl, p2, t);
        auto p = gfx::lerp(a, b, t);
        draw_line_thick(last, p, thickness, color);
        last = p;
    }
}

void Renderer::draw_cubic_bezier_curve(gfx::Vec p1, gfx::Vec p2, gfx::Vec ctl1, gfx::Vec ctl2, float thickness, gfx::Color color) {
    gfx::Vec last = p1;

    for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
        auto a = gfx::lerp(gfx::lerp(p1, ctl1, t), gfx::lerp(ctl1, ctl2, t), t);
        auto b = gfx::lerp(gfx::lerp(ctl1, ctl2, t), gfx::lerp(ctl2, p2, t), t);
        auto p = gfx::lerp(a, b, t);
        draw_line_thick(last, p, thickness, color);
        last = p;
    }
}

void Renderer::clear_background(Color color) {
    auto normalized = color.normalized();
    glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace gfx
