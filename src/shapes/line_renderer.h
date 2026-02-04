#pragma once

#include <gfx/window.h>
#include <gfx/color.h>
#include <gfx/rotation.h>
#include "../opengl.h"

class LineRenderer final {
    const gfx::Surface& m_surface;

    gl::Program m_program;
    gl::VertexArray m_vertex_array;
    gl::Buffer m_vertex_buffer;

public:
    explicit LineRenderer(const gfx::Surface& surface);
    void draw(gfx::Vec start, gfx::Vec end, gfx::Color color, glm::mat4 view);

};
