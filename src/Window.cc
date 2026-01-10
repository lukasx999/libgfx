#include <memory>
#include <print>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <Window.h>
#include <io.h>
#include "WindowImpl.h"
#include "FontImpl.h"

namespace gfx {

Window::Window(int width, int height, const char* window_title, uint8_t flags)
: m_pimpl(std::make_unique<Window::Impl>(width, height, window_title, flags))
{
    library_has_been_initialized = true;
}

Window::~Window() {
    glfwDestroyWindow(m_pimpl->m_window);
    glfwTerminate();
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

Window::Impl::Impl(int width, int height, const char* window_title, uint8_t flags) {

    if (FT_Init_FreeType(&m_ft) != 0)
        throw gfx::Error("failed to initialize ft2");

    if (flags & WindowFlags::Logging) {
        glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
            std::println(stderr, "glfw error: {}", desc);
        });
    }

    if (!glfwInit())
        throw gfx::Error("failed to create window");

    glfwWindowHint(GLFW_RESIZABLE, flags & WindowFlags::Resizable);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (m_window == nullptr)
        throw gfx::Error("failed to create window");

    glfwMakeContextCurrent(m_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(flags & WindowFlags::DisableVsync ? 0 : 1);

    glfwSetInputMode(m_window, GLFW_CURSOR, flags & WindowFlags::DisableCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (flags & WindowFlags::Wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (flags & WindowFlags::Logging)
        glDebugMessageCallback(debug_message_callback, nullptr);

    glfwSetWindowSizeCallback(m_window, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // when setting the window to not be resizable via glfwWindowHint(), glfw may
    // decide to resize the window, so in order to process the resize event before
    // the library user may access the window width/height, we have to poll the events first.
    // we also have to swap buffers exactly three times, probably because the driver
    // is using triple buffering.
    glfwSwapBuffers(m_window);
    glfwSwapBuffers(m_window);
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::Impl::debug_message_callback(
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

gfx::Font Window::Impl::load_font(const char* path) const {
    return gfx::Font(std::make_unique<gfx::Font::Impl>(m_ft, path));
}

} // namespace gfx
