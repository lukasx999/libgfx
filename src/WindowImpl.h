#pragma once

#include <Window.h>
#include "util.h"

namespace gfx {

struct Window::Impl {
    GLFWwindow* m_window;

    Impl(int width, int height, const char *window_title, uint8_t flags);

    static void debug_message_callback(
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *user_param
    );
};

} // namespace gfx
