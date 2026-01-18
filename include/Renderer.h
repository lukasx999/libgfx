#pragma once

#include <functional>
#include <memory>

#include <types.h>
#include <Vec.h>
#include <Rect.h>
#include <Window.h>
#include <Texture.h>
#include <Font.h>
#include <Color.h>

// TODO: support for off-screen rendering without opening a window
// TODO: make texture default ctor public
// TODO: key state rising edge detection
// TODO: check_collision_circle
// TODO: add lerp concept to Animation.h
// TODO: cursor always starts out at 0 0
// TODO: animated clock example
// TODO: font copy/move ctor
// TODO: camera rotation
// TODO: finish gfx::WindowBuilder
// TODO: build system for non-cmake
// TODO: auto-invoke python script for generating shader glue code
// TODO: stencil buffer
// TODO: make texture parameters configurable
// TODO: do copying of textures on the gpu, instead of cpu

namespace gfx {

class Renderer final {
    gfx::Window& m_window;

    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    double m_frame_time = 0.0;
    double m_last_frame = 0.0;
    double m_desired_fps = 0.0;

    using DrawFn = std::function<void()>;

public:
    explicit Renderer(gfx::Window& window);
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    ~Renderer();

    // 0.0 means no limit
    void set_fps(double fps) {
        m_desired_fps = fps;
    }

    [[nodiscard]] gfx::Window& get_window() const {
        return m_window;
    }

    [[nodiscard]] double get_frame_time() const {
        return m_frame_time;
    }

    [[nodiscard]] double get_fps() const {
        return 1.0 / m_frame_time;
    }

    // calls the given function in a draw context, issuing draw calls outside
    // of this context, will result in undefined behavior
    void with_draw_context(DrawFn draw_fn);

    [[nodiscard]] gfx::Texture draw_offscreen(DrawFn draw_fn);

    // calls the given function in a draw loop
    void draw_loop(DrawFn draw_fn);

    [[nodiscard]] gfx::Texture to_texture(DrawFn draw_fn);

    void with_camera(DrawFn draw_fn);

    void set_camera(float center_x, float center_y);

    void set_camera(gfx::Vec vec) {
        set_camera(vec.x, vec.y);
    }

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

    void draw_text(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color);

    void draw_text(gfx::Vec vec, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        draw_text(vec.x, vec.y, text_size, text, font, color);
    }

    void draw_text(float x, float y, int text_size, const std::string& text, const gfx::Font& font, gfx::Color color) {
        draw_text(x, y, text_size, text.c_str(), font, color);
    }

    void draw_text(gfx::Vec vec, int text_size, const std::string& text, const gfx::Font& font, gfx::Color color) {
        draw_text(vec.x, vec.y, text_size, text.c_str(), font, color);
    }

    void draw_text_centered(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        int text_width = font.measure_text(text, text_size);
        draw_text(x - text_width/2.0, y, text_size, text, font, color);
    }

    void clear_background(gfx::Color color);

};

} // namespace gfx
