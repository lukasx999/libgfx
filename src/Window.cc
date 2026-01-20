#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <Window.h>
#include <input.h>
#include "WindowImpl.h"
#include "util.h"

namespace gfx {

Window::Window(int width, int height, const char* window_title, WindowFlags flags)
: m_pimpl(std::make_unique<Window::Impl>(width, height, window_title, flags))
{
    library_has_been_initialized = true;
}

Window::~Window() {
    glfwDestroyWindow(m_pimpl->m_window);
    glfwTerminate();
}

gfx::Vec Window::get_midpoint() const {
    return {
        get_width() / 2.0f,
        get_height() / 2.0f,
    };
}

bool Window::should_close() const {
    return glfwWindowShouldClose(m_pimpl->m_window);
}

void Window::close() {
    glfwSetWindowShouldClose(m_pimpl->m_window, true);
}

const char* Window::get_title() const {
    return glfwGetWindowTitle(m_pimpl->m_window);
}

void Window::set_title(const char* title) {
    glfwSetWindowTitle(m_pimpl->m_window, title);
}

int Window::get_width() const {
    int width;
    glfwGetFramebufferSize(m_pimpl->m_window, &width, nullptr);
    return width;
}

int Window::get_height() const {
    int height;
    glfwGetFramebufferSize(m_pimpl->m_window, nullptr, &height);
    return height;
}

gfx::Rect Window::get_screen_rect() const {
    return {
        0.0f,
        0.0f,
        static_cast<float>(get_width()),
        static_cast<float>(get_height()),
    };
}

double Window::get_time() const {
    return glfwGetTime();
}

gfx::KeyState Window::get_mouse_button_state(MouseButton mb) const {
    return KeyState(glfwGetMouseButton(m_pimpl->m_window, gfx_mouse_button_to_glfw_mouse_button(mb)));
}

gfx::KeyState Window::get_key_state(Key key) const {
    return KeyState(glfwGetKey(m_pimpl->m_window, gfx_key_to_glfw_key(key)));
}

gfx::Vec Window::get_mouse_pos() const {
    double x, y;
    glfwGetCursorPos(m_pimpl->m_window, &x, &y);
    return {
        static_cast<float>(x),
        static_cast<float>(y),
    };
}

int gfx::Window::gfx_mouse_button_to_glfw_mouse_button(MouseButton mb) {
    switch (mb) {
        using enum gfx::MouseButton;
        case Left: return GLFW_MOUSE_BUTTON_LEFT;
        case Right: return GLFW_MOUSE_BUTTON_RIGHT;
        case Middle: return GLFW_MOUSE_BUTTON_MIDDLE;
    }
}

int gfx::Window::gfx_key_to_glfw_key(Key key) {
    switch (key) {
        using enum Key;
        case Escape: return GLFW_KEY_ESCAPE;
        case Space: return GLFW_KEY_SPACE;
        case Num0: return GLFW_KEY_0;
        case Num1: return GLFW_KEY_1;
        case Num2: return GLFW_KEY_2;
        case Num3: return GLFW_KEY_3;
        case Num4: return GLFW_KEY_4;
        case Num5: return GLFW_KEY_5;
        case Num6: return GLFW_KEY_6;
        case Num7: return GLFW_KEY_7;
        case Num8: return GLFW_KEY_8;
        case Num9: return GLFW_KEY_9;
        case A: return GLFW_KEY_A;
        case B: return GLFW_KEY_B;
        case C: return GLFW_KEY_C;
        case D: return GLFW_KEY_D;
        case E: return GLFW_KEY_E;
        case F: return GLFW_KEY_F;
        case G: return GLFW_KEY_G;
        case H: return GLFW_KEY_H;
        case I: return GLFW_KEY_I;
        case J: return GLFW_KEY_J;
        case K: return GLFW_KEY_K;
        case L: return GLFW_KEY_L;
        case M: return GLFW_KEY_M;
        case N: return GLFW_KEY_N;
        case O: return GLFW_KEY_O;
        case P: return GLFW_KEY_P;
        case Q: return GLFW_KEY_Q;
        case R: return GLFW_KEY_R;
        case S: return GLFW_KEY_S;
        case T: return GLFW_KEY_T;
        case U: return GLFW_KEY_U;
        case V: return GLFW_KEY_V;
        case W: return GLFW_KEY_W;
        case X: return GLFW_KEY_X;
        case Y: return GLFW_KEY_Y;
        case Z: return GLFW_KEY_Z;
    }
}

gfx::Font Window::load_font(const char* path) const {
    return m_pimpl->load_font(path);
}

} // namespace gfx
