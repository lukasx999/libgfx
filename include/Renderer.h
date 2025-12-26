#pragma once

#include <functional>
#include <memory>

#include <types.h>
#include <Vec.h>
#include <Window.h>
#include <Texture.h>
#include <Font.h>
#include <Color.h>

// TODO: add global variable for checking that context has been initialized before using textures and so on
// TODO: texture slicing
// TODO: remove ridiculous rotation types
// TODO: merge gfx::Window and gfx::Renderer into one gfx::Context type
// TODO: fix spritesheet
// TODO: add rendering to textures/files using opengl fbo
// TODO: font copy/move ctor
// TODO: camera rotation
// TODO: finish gfx::WindowBuilder
// TODO: build system for non-cmake
// TODO: auto-invoke python script for generating shader glue code
// TODO: merge get_key_state and get_mouse_button_state?
// TODO: stencil buffer
// TODO: make texture parameters configurable
// TODO: hot reloading?
// TODO: do copying of textures on the gpu, instead of cpu

namespace gfx {

class Renderer {
    Window& m_window;

    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    double m_frame_time = 0.0;
    double m_last_frame = 0.0;

    double m_desired_fps = 0.0;

public:
    explicit Renderer(Window& window);
    ~Renderer();

    // 0.0 means no limit
    void set_fps(double fps) {
        m_desired_fps = fps;
    }

    // calls the given function in a draw context, issuing draw calls outside
    // of this context, will result in undefined behavior
    void with_draw_context(std::function<void()> draw_fn);

    // calls the given function in a draw loop
    void draw(std::function<void()> draw_fn) {
        while (!m_window.should_close()) {
            with_draw_context(draw_fn);
        }
    }

    [[nodiscard]] Window& get_window() const {
        return m_window;
    }

    [[nodiscard]] double get_frame_time() const {
        return m_frame_time;
    }

    [[nodiscard]] double get_fps() const {
        return 1.0 / m_frame_time;
    }

    void with_camera(std::function<void()> draw_fn);

    void set_camera(float center_x, float center_y);

    void set_camera(gfx::Vec vec) {
        set_camera(vec.x, vec.y);
    }

    void draw_rectangle(float x, float y, float width, float height, const gfx::IRotation& rotation, gfx::Color color);

    void draw_rectangle(float x, float y, float width, float height, gfx::Color color) {
        draw_rectangle(x, y, width, height, 0_deg, color);
    }

    void draw_rectangle(gfx::Rect rect, gfx::Color color) {
        draw_rectangle(rect.x, rect.y, rect.width, rect.height, 0_deg, color);
    }

    void draw_rectangle(gfx::Rect rect, const gfx::IRotation& rotation, gfx::Color color) {
        draw_rectangle(rect.x, rect.y, rect.width, rect.height, rotation, color);
    }

    void draw_rectangle(gfx::Vec vec, float width, float height, const gfx::IRotation& rotation, gfx::Color color) {
        draw_rectangle(vec.x, vec.y, width, height, rotation, color);
    }

    void draw_rectangle(gfx::Vec vec, float width, float height, gfx::Color color) {
        draw_rectangle(vec.x, vec.y, width, height, 0_deg, color);
    }

    void draw_texture(float x, float y, float width, float height, const gfx::IRotation& rotation, const gfx::Texture& texture);

    void draw_texture(float x, float y, float width, float height, const gfx::Texture& texture) {
        draw_texture(x, y, width, height, 0_deg, texture);
    }

    void draw_texture(gfx::Vec vec, float width, float height, const gfx::IRotation& rotation, const gfx::Texture& texture) {
        draw_texture(vec.x, vec.y, width, height, rotation, texture);
    }

    void draw_texture(gfx::Vec vec, float width, float height, const gfx::Texture& texture) {
        draw_texture(vec.x, vec.y, width, height, 0_deg, texture);
    }

    void draw_texture(gfx::Rect rect, const gfx::IRotation& rotation, const gfx::Texture& texture) {
        draw_texture(rect.x, rect.y, rect.width, rect.height, rotation, texture);
    }

    void draw_texture(gfx::Rect rect, const gfx::Texture& texture) {
        draw_texture(rect.x, rect.y, rect.width, rect.height, 0_deg, texture);
    }

    void draw_texture_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, const gfx::IRotation& rotation, const gfx::Texture& texture);

    void draw_texture_sub(gfx::Rect dest, gfx::Rect src, const gfx::IRotation& rotation, const gfx::Texture& texture) {
        draw_texture_sub(dest.x, dest.y, dest.width, dest.height, src.x, src.y, src.width, src.height, rotation, texture);
    }

    void draw_texture_sub(gfx::Rect dest, gfx::Rect src, const gfx::Texture& texture) {
        draw_texture_sub(dest.x, dest.y, dest.width, dest.height, src.x, src.y, src.width, src.height, 0_deg, texture);
    }

    void draw_circle(float x, float y, float radius, gfx::Color color);

    void draw_circle(gfx::Vec center, float radius, gfx::Color color) {
        draw_circle(center.x, center.y, radius, color);
    }

    void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color);

    void draw_triangle(gfx::Vec v0, gfx::Vec v1, gfx::Vec v2, gfx::Color color) {
        draw_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
    }

    void draw_line(float x0, float y0, float x1, float y1, gfx::Color color);

    void draw_line(gfx::Vec v0, gfx::Vec v1, gfx::Color color) {
        draw_line(v0.x, v0.y, v1.x, v1.y, color);
    }

    void draw_text(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color);

    void draw_text(gfx::Vec vec, int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        draw_text(vec.x, vec.y, text_size, text, font, color);
    }

    [[nodiscard]] gfx::Font load_font(const char* path) const;
    void clear_background(Color color);

};

} // namespace gfx
