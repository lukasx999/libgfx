#pragma once

#include <Window.h>
#include <types.h>
#include "../util.h"

class LineRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

public:
    explicit LineRenderer(gfx::Window& window);
    void draw(float x0, float y0, float x1, float y1, gfx::Color color, glm::mat4 view);

};
