#include <memory>
#include <print>

#include <gfx/gfx.h>

#include "window_impl.h"
#include "font_impl.h"

namespace gfx {

Window::Impl::Impl(int width, int height, const char* title, WindowFlags flags) {

    if (FT_Init_FreeType(&m_ft) != 0)
        throw gfx::Error("failed to initialize ft2");

    if (flags.m_enable_logging) {
        glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
            std::println(stderr, "glfw error: {}", desc);
        });
    }

    if (!glfwInit())
        throw gfx::Error("failed to create window");

    glfwWindowHint(GLFW_RESIZABLE, flags.m_enable_resizing);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
        throw gfx::Error("failed to create window");

    glfwMakeContextCurrent(m_window);

#ifdef USE_GL_GLAD
    gladLoadGL(glfwGetProcAddress);
#endif // USE_GL_GLAD


    // on emscripten the swap interval gets set via emscripten_set_main_loop(), so
    // this results in an error
#ifndef __EMSCRIPTEN__
    glfwSwapInterval(flags.m_enable_vsync);
#endif // __EMSCRIPTEN__


    glfwSetInputMode(m_window, GLFW_CURSOR, flags.m_show_cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

#ifndef __EMSCRIPTEN__
    glEnable(GL_MULTISAMPLE);
#endif // __EMSCRIPTEN__
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // glPolygonMode() and glDebugMessageCallback() are not supported by webgl
#ifndef __EMSCRIPTEN__
    if (flags.m_enable_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (flags.m_enable_logging)
        glDebugMessageCallback(debug_message_callback, nullptr);
#endif // __EMSCRIPTEN__


    glfwSetWindowSizeCallback(m_window, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // TODO: this issue might have been fixed...
    // when setting the window to not be resizable via glfwWindowHint(), glfw may
    // decide to resize the window, so in order to process the resize event before
    // the library user may access the window width/height, we have to poll the events first.
    // we also have to swap buffers exactly three times, probably because the driver
    // is using triple buffering.
    // glfwSwapBuffers(m_window);
    // glfwSwapBuffers(m_window);
    // glfwSwapBuffers(m_window);
    // glfwPollEvents();
}

Window::Impl::~Impl() {
    glfwDestroyWindow(m_window);
    glfwTerminate();

    FT_Done_FreeType(m_ft);
}

void Window::Impl::debug_message_callback(
    [[maybe_unused]] GLenum source,
    [[maybe_unused]] GLenum type,
    [[maybe_unused]] GLuint id,
    [[maybe_unused]] GLenum severity,
    [[maybe_unused]] GLsizei length,
    const GLchar *message,
    [[maybe_unused]] const void *user_param
) {

#ifndef __EMSCRIPTEN__
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
#endif // __EMSCRIPTEN__
    std::println(stderr, "opengl error: {}", message);
}

gfx::Font Window::Impl::load_font(const char* path) const {
    return gfx::Font(std::make_unique<gfx::Font::Impl>(m_ft, path));
}

} // namespace gfx
