#pragma once

#include <memory>

#include <types.h>
#include <Vec.h>
#include <io.h>

namespace gfx {

enum WindowFlags : uint8_t {
    None          = 1 << 0,
    Resizable     = 1 << 1,
    DisableCursor = 1 << 2,
    DisableVsync  = 1 << 3,
};

class Window {
    friend class Renderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Window(int width, int height, const char* window_title, uint8_t flags);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    ~Window();

    [[nodiscard]] bool should_close() const;
    void close();
    [[nodiscard]] const char* get_title() const;
    void set_title(const char* title);
    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    // returns the current time in seconds
    [[nodiscard]] double get_time() const;
    [[nodiscard]] KeyState get_mouse_button_state(MouseButton mb) const;
    [[nodiscard]] KeyState get_key_state(Key key) const;
    [[nodiscard]] gfx::Vec get_mouse_pos() const;

private:
    [[nodiscard]] static int gfx_mouse_button_to_glfw_mouse_button(MouseButton mb);
    [[nodiscard]] static int gfx_key_to_glfw_key(Key key);


};

// class WindowBuilder {
//     int m_width = 1600;
//     int m_height = 900;
//     const char* m_title = "libgfx application";
//
// public:
//     WindowBuilder() = default;
//
//     WindowBuilder& set_width(int width) {
//         m_width = width;
//         return *this;
//     }
//
//     WindowBuilder& set_height(int height) {
//         m_height = height;
//         return *this;
//     }
//
//     WindowBuilder& set_title(const char* title) {
//         m_title = title;
//         return *this;
//     }
//
//     WindowBuilder& resizable(bool resizable) {
//         return *this;
//     }
//
//     WindowBuilder& enable_cursor(bool enable_cursor) {
//         return *this;
//     }
//
//     WindowBuilder& enable_vsync(bool vsync) {
//         return *this;
//     }
//
//     [[nodiscard]] Window build() const {
//         return Window { m_width, m_height, m_title, 0 };
//     }
//
// };

} // namespace gfx
