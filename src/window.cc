#include <thread>
#include <tuple>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gfx/window.h>
#include <gfx/input.h>
#include "window_impl.h"
#include "opengl.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__

namespace gfx {

Window::Window(int width, int height, const char* title, WindowFlags flags)
    : m_pimpl(std::make_unique<Window::Impl>(width, height, title, flags))
    , m_renderer(*this)
{ }

// the pimpl pattern requires the destructor to "see" the complete
// type of the Impl structure
Window::~Window() = default;

bool Window::should_close() const {
    return glfwWindowShouldClose(m_pimpl->m_window);
}

void Window::close() {
    glfwSetWindowShouldClose(m_pimpl->m_window, true);
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

double Window::get_time() const {
    return glfwGetTime();
}

gfx::Texture Window::draw_offscreen(DrawFn draw_fn) {
    return m_renderer.to_texture([&] {
        draw_fn(m_renderer);
    });
}

void Window::draw_loop(DrawFn draw_fn) {

#ifdef __EMSCRIPTEN__
    std::tuple<gfx::Window&, DrawFn> tuple(*this, draw_fn);

    emscripten_set_main_loop_arg([](void* data) {
        auto& [window, fn] = *static_cast<decltype(tuple)*>(data);
        window.with_draw_loop_context(fn);
    }, &tuple, 0, true);
#else
    while (!should_close())
        with_draw_loop_context(draw_fn);
#endif // __EMSCRIPTEN__

}

void Window::with_draw_loop_context(DrawFn draw_fn) {

    double frame_start = glfwGetTime();
    m_frame_time = frame_start - m_last_frame;
    m_last_frame = frame_start;

    draw_fn(m_renderer);

    glfwSwapBuffers(m_pimpl->m_window);
    glfwPollEvents();

    // sleep for the rest of the frame to keep the desired framerate steady
    if (m_desired_fps == 0.0) return;
    double time_so_far = glfwGetTime() - frame_start;

    double time_to_sleep = 1.0 / m_desired_fps - time_so_far;
    std::chrono::duration<double> duration(time_to_sleep);
    std::this_thread::sleep_for(duration);
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

    std::unreachable();
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

    std::unreachable();
}

gfx::Font Window::load_font(const char* path) const {
    return m_pimpl->load_font(path);
}

} // namespace gfx
