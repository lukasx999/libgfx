#pragma once

#include <gfx/window.h>
#include <gfx/color.h>
#include <gfx/rotation.h>
#include "../opengl.h"

class TriangleRenderer final {
    const gfx::Surface& m_surface;

    gl::Program m_program;
    gl::VertexArray m_vertex_array;
    gl::Buffer m_vertex_buffer;

public:
    explicit TriangleRenderer(const gfx::Surface& surface);
    void draw(gfx::Vec a, gfx::Vec b, gfx::Vec c, gfx::Color color, glm::mat4 view);

};
