#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gfx/window.h>
#include <gfx/font.h>
#include "opengl.h"

namespace gfx {

struct Window::Impl {
    GLFWwindow* m_window;

    FT_Library m_ft;

    Impl(int width, int height, const char* title, WindowFlags flags);
    ~Impl();

    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;

    [[nodiscard]] gfx::Font load_font(const char* path) const;

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
