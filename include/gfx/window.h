#pragma once

#include <memory>
#include <functional>

#include <gfx/surface.h>
#include <gfx/renderer.h>
#include <gfx/rect.h>
#include <gfx/vec.h>
#include <gfx/texture.h>
#include <gfx/font.h>
#include <gfx/input.h>

namespace gfx {

struct WindowFlags {
    bool m_enable_resizing  = false;
    bool m_show_cursor      = true;
    bool m_enable_vsync     = true;
    bool m_enable_logging   = true;
    bool m_enable_wireframe = false;

    WindowFlags& enable_resizing(bool value) {
        m_enable_resizing = value;
        return *this;
    }

    WindowFlags& show_cursor(bool value) {
        m_show_cursor = value;
        return *this;
    }

    WindowFlags& enable_vsync(bool value) {
        m_enable_vsync = value;
        return *this;
    }

    WindowFlags& enable_logging(bool value) {
        m_enable_logging = value;
        return *this;
    }

    WindowFlags& enable_wireframe(bool value) {
        m_enable_wireframe = value;
        return *this;
    }

};

class Window final : public gfx::Surface {
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    double m_frame_time = 0.0;
    double m_last_frame = 0.0;
    double m_desired_fps = 0.0;

    gfx::Renderer m_renderer;

public:
    using DrawFn = std::function<void(gfx::Renderer&)>;

    Window(int width, int height, const char* title, WindowFlags flags = {});
    ~Window();

    [[nodiscard]] double get_frame_time() const {
        return m_frame_time;
    }

    [[nodiscard]] double get_fps() const {
        return 1.0 / m_frame_time;
    }

    // 0.0 means no limit
    void set_fps(double fps) {
        m_desired_fps = fps;
    }

    [[nodiscard]] int get_width() const override;
    [[nodiscard]] int get_height() const override;

    void set_title(const char* title);
    void close();

    // draw into a texture without opening a window
    [[nodiscard]] gfx::Texture draw_offscreen(DrawFn draw_fn);

    // calls the given function in a draw loop
    void draw_loop(DrawFn draw_fn);

    // returns the current time in seconds
    [[nodiscard]] double get_time() const;

    [[nodiscard]] KeyState get_mouse_button_state(MouseButton mb) const;
    [[nodiscard]] KeyState get_key_state(Key key) const;
    [[nodiscard]] gfx::Vec get_mouse_pos() const;

    [[nodiscard]] gfx::Font load_font(const char* path) const;

private:
    [[nodiscard]] static int gfx_mouse_button_to_glfw_mouse_button(MouseButton mb);
    [[nodiscard]] static int gfx_key_to_glfw_key(Key key);

    // calls the given function in a draw context, issuing draw calls outside
    // of this context will result in undefined behavior
    void with_draw_loop_context(DrawFn draw_fn);

    [[nodiscard]] bool should_close() const;

};

} // namespace gfx
