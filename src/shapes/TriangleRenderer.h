#pragma once

#include <Window.h>
#include <Color.h>
#include <types.h>

#include "../opengl.h"

class TriangleRenderer final {
    const gfx::Window& m_window;

    Program m_program;
    VertexArray m_vertex_array;
    Buffer m_vertex_buffer;

public:
    explicit TriangleRenderer(const gfx::Window& window);
    void draw(gfx::Vec a, gfx::Vec b, gfx::Vec c, gfx::Color color, glm::mat4 view);

};
