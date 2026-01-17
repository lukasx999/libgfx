#pragma once

#include <Window.h>
#include <types.h>
#include "../opengl.h"

class LineRenderer final {
    const gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

public:
    explicit LineRenderer(const gfx::Window& window);
    void draw(gfx::Vec start, gfx::Vec end, gfx::Color color, glm::mat4 view);

};
