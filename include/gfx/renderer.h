#pragma once

#include <functional>
#include <memory>
#include <string_view>

#include <gfx/surface.h>
#include <gfx/texture.h>
#include <gfx/vec.h>
#include <gfx/rect.h>
#include <gfx/types.h>
#include <gfx/color.h>
#include <gfx/font.h>

// TODO: make pkg-config file work
// TODO: fix library initializer check segfault in gfx::Texture (gl::Texture is constructed before the check)
// TODO: fix window dtor
// TODO: camera rotation
// TODO: texture enum class for format (rgb, rgba, ...)
// TODO: key state rising edge detection
// TODO: check_collision_circle
// TODO: add lerp concept to Animation.h
// TODO: cursor always starts out at 0 0
// TODO: animated clock example
// TODO: font copy/move ctor
// TODO: build system for non-cmake
// TODO: stencil buffer
// TODO: make texture parameters configurable

namespace gfx {

class Renderer final {
    const gfx::Surface& m_surface;

    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    using DrawFn = std::function<void()>;

    // TODO: hide this ctor
    explicit Renderer(const gfx::Surface& surface);

    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    [[nodiscard]] const gfx::Surface& get_surface() const {
        return m_surface;
    }

    [[nodiscard]] gfx::Texture to_texture(DrawFn draw_fn);

    void with_camera(DrawFn draw_fn);

    void set_camera(gfx::Vec vec) {
        set_camera(vec.x, vec.y);
    }

    void set_camera(float center_x, float center_y);

    void draw_rectangle(gfx::Rect rect, gfx::Rotation rotation, gfx::Color color);

    void draw_rectangle(float x, float y, float width, float height, gfx::Rotation rotation, gfx::Color color) {
        draw_rectangle({ x, y, width, height }, rotation, color);
    }

    void draw_rectangle(float x, float y, float width, float height, gfx::Color color) {
        draw_rectangle(x, y, width, height, 0_deg, color);
    }

    void draw_rectangle(gfx::Rect rect, gfx::Color color) {
        draw_rectangle(rect.x, rect.y, rect.width, rect.height, 0_deg, color);
    }

    void draw_rectangle(gfx::Vec vec, float width, float height, gfx::Rotation rotation, gfx::Color color) {
        draw_rectangle(vec.x, vec.y, width, height, rotation, color);
    }

    void draw_rectangle(gfx::Vec vec, float width, float height, gfx::Color color) {
        draw_rectangle(vec.x, vec.y, width, height, 0_deg, color);
    }

    void draw_texture(float x, float y, float width, float height, gfx::Rotation rotation, const gfx::Texture& texture);

    void draw_texture(float x, float y, float width, float height, const gfx::Texture& texture) {
        draw_texture(x, y, width, height, 0_deg, texture);
    }

    void draw_texture(gfx::Vec vec, float width, float height, gfx::Rotation rotation, const gfx::Texture& texture) {
        draw_texture(vec.x, vec.y, width, height, rotation, texture);
    }

    void draw_texture(gfx::Vec vec, float width, float height, const gfx::Texture& texture) {
        draw_texture(vec.x, vec.y, width, height, 0_deg, texture);
    }

    void draw_texture(gfx::Rect rect, gfx::Rotation rotation, const gfx::Texture& texture) {
        draw_texture(rect.x, rect.y, rect.width, rect.height, rotation, texture);
    }

    void draw_texture(gfx::Rect rect, const gfx::Texture& texture) {
        draw_texture(rect.x, rect.y, rect.width, rect.height, 0_deg, texture);
    }

    void draw_texture_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, gfx::Rotation rotation, const gfx::Texture& texture);

    void draw_texture_sub(gfx::Rect dest, gfx::Rect src, gfx::Rotation rotation, const gfx::Texture& texture) {
        draw_texture_sub(dest.x, dest.y, dest.width, dest.height, src.x, src.y, src.width, src.height, rotation, texture);
    }

    void draw_texture_sub(gfx::Rect dest, gfx::Rect src, const gfx::Texture& texture) {
        draw_texture_sub(dest.x, dest.y, dest.width, dest.height, src.x, src.y, src.width, src.height, 0_deg, texture);
    }

    void draw_circle(gfx::Vec center, float radius, gfx::Color color);

    void draw_circle(float x, float y, float radius, gfx::Color color) {
        draw_circle({ x, y }, radius, color);
    }

    void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color) {
        draw_triangle({ x0, y0 }, { x1, y1 }, { x2, y2 }, color);
    }

    void draw_triangle(gfx::Vec a, gfx::Vec b, gfx::Vec c, gfx::Color color);

    void draw_line(gfx::Vec a, gfx::Vec b, gfx::Color color);

    void draw_line(float x0, float y0, float x1, float y1, gfx::Color color) {
        draw_line({ x0, y0 }, { x1, y1 }, color);
    }

    void draw_text(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color);

    void draw_text(float x, float y, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        draw_text({ x, y }, fontsize, text, font, color);
    }

    void draw_text_centered(gfx::Vec center, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        int text_width = font.measure_text(text, fontsize);
        draw_text({ center.x - text_width/2.0f, center.y }, fontsize, text, font, color);
    }

    void draw_text_centered(float x, float y, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        draw_text_centered({ x, y }, fontsize, text, font, color);
    }

    void clear_background(gfx::Color color);

};

} // namespace gfx
