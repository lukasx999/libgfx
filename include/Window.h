#pragma once

#include "Font.h"
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
    Logging       = 1 << 4,
    Wireframe     = 1 << 5,
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
    [[nodiscard]] const char* get_title() const;
    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
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
