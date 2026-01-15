#pragma once

#include <Window.h>
#include <types.h>

#include "../util.h"

class TriangleRenderer final {
    const gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

public:
    explicit TriangleRenderer(const gfx::Window& window);
    void draw(gfx::Vec a, gfx::Vec b, gfx::Vec c, gfx::Color color, glm::mat4 view);

};
