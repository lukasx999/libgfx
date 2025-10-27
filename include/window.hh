#pragma once

#include <print>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <types.hh>
#include <vec.hh>

namespace gfx {

enum WindowFlags : uint8_t {
    None          = 1 << 0,
    Resizable     = 1 << 1,
    DisableCursor = 1 << 2,
    DisableVsync  = 1 << 3,
};

class Window {
    friend class Renderer;
    GLFWwindow* m_window;

public:
    Window(int width, int height, const char* window_title, uint8_t flags) {
        m_window = init_glfw(width, height, window_title, flags);
    }

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    [[nodiscard]] bool should_close() const {
        return glfwWindowShouldClose(m_window);
    }

    void close() {
        glfwSetWindowShouldClose(m_window, true);
    }

    [[nodiscard]] const char* get_title() const {
        return glfwGetWindowTitle(m_window);
    }

    void set_title(const char* title) {
        glfwSetWindowTitle(m_window, title);
    }

    [[nodiscard]] int get_width() const {
        int width;
        glfwGetFramebufferSize(m_window, &width, nullptr);
        return width;
    }

    [[nodiscard]] int get_height() const {
        int height;
        glfwGetFramebufferSize(m_window, nullptr, &height);
        return height;
    }

    [[nodiscard]] double get_time() const {
        return glfwGetTime();
    }

    [[nodiscard]] KeyState get_mouse_button_state(MouseButton mb) const {
        return KeyState(glfwGetMouseButton(m_window, gfx_mouse_button_to_glfw_mouse_button(mb)));
    }

    [[nodiscard]] KeyState get_key_state(Key key) const {
        return KeyState(glfwGetKey(m_window, gfx_key_to_glfw_key(key)));
    }

    [[nodiscard]] gfx::Vec get_mouse_pos() const {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        return {
            static_cast<float>(x),
            static_cast<float>(y),
        };
    }

private:
    [[nodiscard]] static GLFWwindow* init_glfw(int width, int height, const char* window_title, uint8_t flags);
    [[nodiscard]] static int gfx_mouse_button_to_glfw_mouse_button(MouseButton mb);
    [[nodiscard]] static int gfx_key_to_glfw_key(Key key);

    static void debug_message_callback(
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *user_param
    ) {

        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
        std::println(stderr, "opengl error: {}", message);
    }

};

class WindowBuilder {
    int m_width = 1600;
    int m_height = 900;
    const char* m_title = "libgfx application";

public:
    WindowBuilder() = default;

    WindowBuilder& set_width(int width) {
        m_width = width;
        return *this;
    }

    WindowBuilder& set_height(int height) {
        m_height = height;
        return *this;
    }

    WindowBuilder& set_title(const char* title) {
        m_title = title;
        return *this;
    }

    WindowBuilder& resizable(bool resizable) {
        return *this;
    }

    WindowBuilder& enable_cursor(bool enable_cursor) {
        return *this;
    }

    WindowBuilder& enable_vsync(bool vsync) {
        return *this;
    }

    [[nodiscard]] Window build() const {
        return Window { m_width, m_height, m_title, 0 };
    }

};

} // namespace gfx
