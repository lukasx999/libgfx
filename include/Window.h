#pragma once

#include <memory>

#include "Font.h"
#include "Rect.h"
#include <types.h>
#include <Vec.h>
#include <input.h>

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

class Window final {
    friend class Renderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Window(int width, int height, const char* window_title, WindowFlags flags);
    Window(int width, int height, const char* window_title);
    ~Window();
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] bool should_close() const;
    [[nodiscard]] const char* get_title() const;
    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] gfx::Rect get_screen_rect() const;
    void set_title(const char* title);
    void close();
    // returns the current time in seconds
    [[nodiscard]] double get_time() const;
    [[nodiscard]] KeyState get_mouse_button_state(MouseButton mb) const;
    [[nodiscard]] KeyState get_key_state(Key key) const;
    [[nodiscard]] gfx::Vec get_mouse_pos() const;
    [[nodiscard]] gfx::Font load_font(const char* path) const;

private:
    [[nodiscard]] static int gfx_mouse_button_to_glfw_mouse_button(MouseButton mb);
    [[nodiscard]] static int gfx_key_to_glfw_key(Key key);

};

} // namespace gfx
