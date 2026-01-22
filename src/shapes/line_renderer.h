#pragma once

#include <window.h>
#include <color.h>
#include <types.h>
#include "../opengl.h"

class LineRenderer final {
    const gfx::Surface& m_surface;

    gl::Program m_program;
    gl::VertexArray m_vertex_array;
    gl::Buffer m_vertex_buffer;

public:
    explicit LineRenderer(const gfx::Surface& surface);
    void draw(gfx::Vec a, gfx::Vec b, gfx::Color color, glm::mat4 view);

};
