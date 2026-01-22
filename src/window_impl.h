#pragma once

#include <window.h>
#include "font.h"
#include "opengl.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GLFW/glfw3.h>

namespace gfx {

struct Window::Impl {
    GLFWwindow* m_window;

    FT_Library m_ft;

    Impl(int width, int height, const char* title, WindowFlags flags);

    ~Impl() {
        FT_Done_FreeType(m_ft);
    }

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
