#pragma once

#include <array>
#include <functional>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/rectangle.hh>
#include <detail/circle.hh>
#include <detail/triangle.hh>
#include <detail/texture.hh>
#include <detail/line.hh>
#include <detail/text.hh>

#include <types.hh>
#include <vec.hh>
#include <window.hh>

// TODO: fix glfw resizing window
// TODO: render text below x/y
// TODO: build system for non-cmake
// TODO: static linking
// TODO: auto-invoke python script for generating shader glue code
// TODO: catch2 unit tests
// TODO: tui for statistics
// TODO: fix circle rendering (batching)
// TODO: anti-aliasing
// TODO: disable opengl/glfw logs
// TODO: asset packer
// TODO: merge get_key_state and get_mouse_button_state?

namespace gfx {

class Renderer {
    Window& m_window;

    detail::RectangleRenderer m_rectangle;
    detail::CircleRenderer m_circle;
    detail::TriangleRenderer m_triangle;
    detail::TextureRenderer m_texture;
    detail::LineRenderer m_line;
    detail::TextRenderer m_text;

    std::array<std::reference_wrapper<detail::IDeferredRenderer>, 4> m_deferred_renderers {
        m_rectangle,
        m_triangle,
        m_texture,
        m_line
    };

    double m_frame_time = 0.0;
    double m_last_frame = 0.0;

    double m_desired_fps = 0.0;

    glm::mat4 m_view_default = gen_view_matrix(
        m_window,
        m_window.get_width() / 2.0f,
        m_window.get_height() / 2.0f
    );
    glm::mat4 m_view_camera = m_view_default;
    glm::mat4 m_view_active = m_view_default;

public:
    explicit Renderer(Window& window)
        : m_window(window)
        , m_rectangle(m_window)
        , m_circle(m_window)
        , m_triangle(m_window)
        , m_texture(m_window)
        , m_line(m_window)
        , m_text(m_window)
    { }

    // 0.0 means no limit
    void set_fps(double fps) {
        m_desired_fps = fps;
    }

    // calls the given function in a draw context, issuing draw calls outside
    // of this context, will result in undefined behavior
    void with_draw_context(std::function<void()> draw_fn) {

        // calculate frame time
        double frame_start = glfwGetTime();
        m_frame_time = frame_start - m_last_frame;
        m_last_frame = frame_start;

        draw_fn();

        flush();
        glfwSwapBuffers(m_window.m_window);
        glfwPollEvents();

        // sleep for the rest of the frame to keep the desired framerate steady
        if (m_desired_fps == 0.0) return;
        double frame_end = glfwGetTime() - frame_start;

        struct timespec ts{};
        ts.tv_nsec = (1.0 / m_desired_fps - frame_end) * 1e9,
        nanosleep(&ts, nullptr);
    }

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

    void with_camera(std::function<void()> draw_fn) {
        m_view_active = m_view_camera;
        draw_fn();
        m_view_active = m_view_default;
    }

    // TODO: rotation
    void set_camera(float center_x, float center_y) {
        m_view_camera = gen_view_matrix(m_window, center_x, center_y);
    }

    void set_camera(gfx::Vec vec) {
        set_camera(vec.x, vec.y);
    }

    void draw_rectangle(
        float x,
        float y,
        float width,
        float height,
        const gfx::IRotation& rotation,
        gfx::Color color
    ) {
        flush_all_except(m_rectangle);
        m_rectangle.draw(x, y, width, height, rotation, color, m_view_active);
    }

    void draw_rectangle(float x, float y, float width, float height, gfx::Color color) {
        draw_rectangle(x, y, width, height, 0_deg, color);
    }

    void draw_rectangle(gfx::Rect rect, gfx::Color color) {
        draw_rectangle(rect.x, rect.y, rect.width, rect.height, 0_deg, color);
    }

    void draw_rectangle(gfx::Rect rect, const gfx::IRotation& rotation, gfx::Color color) {
        draw_rectangle(rect.x, rect.y, rect.width, rect.height, rotation, color);
    }

    void draw_rectangle(
        gfx::Vec vec,
        float width,
        float height,
        const gfx::IRotation& rotation,
        gfx::Color color
    ) {
        draw_rectangle(vec.x, vec.y, width, height, rotation, color);
    }

    void draw_rectangle(gfx::Vec vec, float width, float height, gfx::Color color) {
        draw_rectangle(vec.x, vec.y, width, height, 0_deg, color);
    }

    void draw_texture(
        float x,
        float y,
        float width,
        float height,
        const gfx::IRotation& rotation,
        const gfx::Texture& texture
    ) {
        flush_all_except(m_texture);
        m_texture.draw(x, y, width, height, rotation, texture, m_view_active);
    }

    void draw_texture(
        gfx::Vec vec,
        float width,
        float height,
        const gfx::IRotation& rotation,
        const gfx::Texture& texture
    ) {
        draw_texture(vec.x, vec.y, width, height, rotation, texture);
    }

    void draw_circle(float x, float y, float radius, gfx::Color color) {
        flush();
        m_circle.draw(x, y, radius, color, m_view_active);
    }

    void draw_circle(gfx::Vec center, float radius, gfx::Color color) {
        draw_circle(center.x, center.y, radius, color);
    }

    void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color) {
        flush_all_except(m_triangle);
        m_triangle.draw(x0, y0, x1, y1, x2, y2, color, m_view_active);
    }

    void draw_triangle(gfx::Vec v0, gfx::Vec v1, gfx::Vec v2, gfx::Color color) {
        draw_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
    }

    void draw_line(float x0, float y0, float x1, float y1, gfx::Color color) {
        flush_all_except(m_line);
        m_line.draw(x0, y0, x1, y1, color, m_view_active);
    }

    void draw_line(gfx::Vec v0, gfx::Vec v1, gfx::Color color) {
        draw_line(v0.x, v0.y, v1.x, v1.y, color);
    }

    void draw_text(float x, float y, unsigned int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        flush();
        m_text.draw(x, y, text_size, text, font, color, m_view_active);
    }

    void draw_text(gfx::Vec vec, unsigned int text_size, const char* text, const gfx::Font& font, gfx::Color color) {
        draw_text(vec.x, vec.y, text_size, text, font, color);
    }

    [[nodiscard]] gfx::Font load_font(const char* path) const {
        return m_text.load_font(path);
    }

    void clear_background(Color color) {
        auto normalized = color.normalized();
        glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    void flush() {
        for (auto& rd : m_deferred_renderers) {
            rd.get().flush();
        }
    }

    // make sure all other shapes have already been drawn to the screen,
    // as otherwise the drawing order will be incorrect, leading weird
    // overlapping shapes
    //
    // therefore we have to flush all other shapes before drawing the current one
    //
    // this has the unfortunate effect of breaking batching optimizations
    // when using interleaved rendering when drawing a lot of shapes
    void flush_all_except(const detail::IDeferredRenderer& exception) {
        for (auto& rd : m_deferred_renderers) {
            if (reinterpret_cast<detail::IDeferredRenderer*>(&rd) == &exception)
                continue;
            rd.get().flush();
        }
    }

    [[nodiscard]] static constexpr
    glm::mat4 gen_view_matrix(const Window& window, float center_x, float center_y) {
        glm::vec3 camera_position(
            center_x - window.get_width() / 2.0f,
            center_y - window.get_height() / 2.0f,
            0.0f
        );
        glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);
        return view;
    }

};

} // namespace gfx
