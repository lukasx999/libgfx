#pragma once

#include <Window.h>
#include <Color.h>
#include <types.h>
#include "../opengl.h"

class LineRenderer final {
    const gfx::Window& m_window;

    gl::Program m_program;
    gl::VertexArray m_vertex_array;
    gl::Buffer m_vertex_buffer;

public:
    explicit LineRenderer(const gfx::Window& window);
    void draw(gfx::Vec a, gfx::Vec b, gfx::Color color, glm::mat4 view);

};
