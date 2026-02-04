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

// TODO: hot reloading
// TODO: fix broken text rotation: every character gets rotated
// TODO: do copying of textures on the gpu, so there's no need for glGetTexImage anymore
// TODO: finish wasm port
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

    void draw_rectangle(float x, float y, float width, float height, auto&&... args) {
        draw_rectangle({ x, y, width, height }, std::forward<decltype(args)>(args)...);
    }

    void draw_rectangle(gfx::Vec v, float width, float height, auto&&... args) {
        draw_rectangle({ v.x, v.y, width, height }, std::forward<decltype(args)>(args)...);
    }

    void draw_texture(gfx::Rect rect, const gfx::Texture& texture, gfx::Rotation rotation=0_deg);

    void draw_texture(float x, float y, float width, float height, auto&&... args) {
        draw_texture({ x, y, width, height }, std::forward<decltype(args)>(args)...);
    }

    void draw_texture(gfx::Vec v, float width, float height, auto&&... args) {
        draw_texture({ v.x, v.y, width, height }, std::forward<decltype(args)>(args)...);
    }

    void draw_texture_sub(gfx::Rect dest, gfx::Rect src, const gfx::Texture& texture, gfx::Rotation rotation=0_deg);

    void draw_texture_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, auto&&... args) {
        draw_texture_sub({ dest_x, dest_y, dest_width, dest_height }, { src_x, src_y, src_width, src_height }, std::forward<decltype(args)>(args)...);
    }

    void draw_circle(gfx::Vec center, float radius, gfx::Color color);

    void draw_circle(float x, float y, auto&&... args) {
        draw_circle({ x, y }, std::forward<decltype(args)>(args)...);
    }

    void draw_triangle(gfx::Vec a, gfx::Vec b, gfx::Vec c, gfx::Color color);

    void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color) {
        draw_triangle({ x0, y0 }, { x1, y1 }, { x2, y2 }, color);
    }

    void draw_line(gfx::Vec start, gfx::Vec end, gfx::Color color);

    void draw_line(float x0, float y0, float x1, float y1, gfx::Color color) {
        draw_line({ x0, y0 }, { x1, y1 }, color);
    }

    void draw_line_thick(gfx::Vec start, gfx::Vec end, float thickness, gfx::Color color);

    void draw_text(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color, gfx::Rotation rotation=0_deg);

    void draw_text(float x, float y, auto&&... args) {
        draw_text({ x, y }, std::forward<decltype(args)>(args)...);
    }

    void draw_text_centered_x(gfx::Vec center, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color, gfx::Rotation rotation=0_deg) {
        int text_width = font.measure_text(text, fontsize);
        draw_text({ center.x - text_width/2.0f, center.y }, fontsize, text, font, color, rotation);
    }

    void draw_text_centered_x(float x, float y, auto&&... args) {
        draw_text_centered_x({ x, y }, std::forward<decltype(args)>(args)...);
    }

    void draw_text_centered(gfx::Vec center, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color, gfx::Rotation rotation=0_deg) {
        int text_width = font.measure_text(text, fontsize);
        draw_text({ center.x - text_width/2.0f, center.y - fontsize/2.0f }, fontsize, text, font, color, rotation);
    }

    void draw_text_centered(float x, float y, auto&&... args) {
        draw_text_centered({ x, y }, std::forward<decltype(args)>(args)...);
    }

    void clear_background(gfx::Color color);

};

} // namespace gfx
