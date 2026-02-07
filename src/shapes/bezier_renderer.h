#pragma once

#include <gfx/color.h>
#include <gfx/surface.h>
#include "../opengl.h"

class BezierRenderer final {
    const gfx::Surface& m_surface;

    gl::VertexArray m_vertex_array;
    gl::Program m_program;
    gl::Buffer m_vertex_buffer;

public:
    explicit BezierRenderer(const gfx::Surface& surface);

    void draw(gfx::Vec a, gfx::Vec b, gfx::Vec control, gfx::Color color, glm::mat4 view);

};
