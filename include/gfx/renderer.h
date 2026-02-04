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

// TODO: do copying of textures on the gpu, so there's no need for glGetTexImage anymore
// TODO: finish wasm port
// TODO: default params for draw methods
// TODO: systems for coordinating animations
// TODO: cmake fetchcontent for glm
// TODO: dynamic surface resizing?
// TODO: options for opengl function loaders
// TODO: stencil buffer
// TODO: draw from spritesheet directly
// TODO: clean up vector operations
// TODO: detect default font (via fontconfig)
// TODO: make pkg-config file work
// TODO: fix rotation types
// TODO: let glPixelStorei() only affect texture ctor
// TODO: check_collision_circle
// TODO: cursor always starts out at 0 0
// TODO: font copy/move ctor
// TODO: make texture parameters configurable

namespace gfx {

class Window;
class ExternalContext;

class Renderer final {
    const gfx::Surface& m_surface;

    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    friend gfx::Window;
    friend gfx::ExternalContext;

    explicit Renderer(const gfx::Surface& surface);

public:
    using DrawFn = std::function<void()>;

    ~Renderer();

    [[nodiscard]] const gfx::Surface& get_surface() const {
        return m_surface;
    }

    [[nodiscard]] gfx::Texture to_texture(DrawFn draw_fn);

    void with_camera(DrawFn draw_fn);

    void set_camera(gfx::Vec center, gfx::Rotation rotation=0_deg, float scale=1.0f);

    void set_camera(float center_x, float center_y, gfx::Rotation rotation=0_deg, float scale=1.0f) {
        set_camera({ center_x, center_y }, rotation, scale);
    }

    void draw_rectangle(gfx::Rect rect, gfx::Color color, gfx::Rotation rotation=0_deg);

    void draw_rectangle(float x, float y, float width, float height, gfx::Color color, gfx::Rotation rotation=0_deg) {
        draw_rectangle({ x, y, width, height }, color, rotation);
    }

    void draw_rectangle(gfx::Vec v, float width, float height, gfx::Color color, gfx::Rotation rotation=0_deg) {
        draw_rectangle({ v.x, v.y, width, height }, color, rotation);
    }

    void draw_texture(gfx::Rect rect, gfx::Rotation rotation, const gfx::Texture& texture);

    void draw_texture(float x, float y, float width, float height, gfx::Rotation rotation, const gfx::Texture& texture) {
        draw_texture({ x, y, width, height }, rotation, texture);
    }

    void draw_texture(float x, float y, float width, float height, const gfx::Texture& texture) {
        draw_texture(x, y, width, height, 0_deg, texture);
    }

    void draw_texture(gfx::Vec vec, float width, float height, gfx::Rotation rotation, const gfx::Texture& texture) {
        draw_texture(vec.x, vec.y, width, height, rotation, texture);
    }

    void draw_texture(gfx::Vec vec, float width, float height, const gfx::Texture& texture) {
        draw_texture(vec.x, vec.y, width, height, 0_deg, texture);
    }

    void draw_texture(gfx::Rect rect, const gfx::Texture& texture) {
        draw_texture(rect.x, rect.y, rect.width, rect.height, 0_deg, texture);
    }

    void draw_texture_sub(gfx::Rect dest, gfx::Rect src, gfx::Rotation rotation, const gfx::Texture& texture);

    void draw_texture_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, gfx::Rotation rotation, const gfx::Texture& texture) {
        draw_texture_sub({ dest_x, dest_y, dest_width, dest_height }, { src_x, src_y, src_width, src_height }, rotation, texture);
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

    void draw_line(gfx::Vec start, gfx::Vec end, gfx::Color color);

    void draw_line(float x0, float y0, float x1, float y1, gfx::Color color) {
        draw_line({ x0, y0 }, { x1, y1 }, color);
    }

    void draw_line_thick(gfx::Vec start, gfx::Vec end, float thickness, gfx::Color color);

    void draw_text(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color);

    void draw_text(float x, float y, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        draw_text({ x, y }, fontsize, text, font, color);
    }

    void draw_text_centered_x(gfx::Vec center, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        int text_width = font.measure_text(text, fontsize);
        draw_text({ center.x - text_width/2.0f, center.y }, fontsize, text, font, color);
    }

    void draw_text_centered_x(float x, float y, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        draw_text_centered_x({ x, y }, fontsize, text, font, color);
    }

    void draw_text_centered(gfx::Vec center, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        int text_width = font.measure_text(text, fontsize);
        draw_text({ center.x - text_width/2.0f, center.y - fontsize/2.0f }, fontsize, text, font, color);
    }

    void draw_text_centered(float x, float y, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color) {
        draw_text_centered({ x, y }, fontsize, text, font, color);
    }

    void clear_background(gfx::Color color);

};

} // namespace gfx
