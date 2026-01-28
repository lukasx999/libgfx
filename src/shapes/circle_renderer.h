#pragma once

#include <array>

#include <gfx/types.h>
#include <gfx/color.h>
#include <gfx/window.h>
#include "../opengl.h"

class CircleRenderer final {
    const gfx::Surface& m_surface;

    gl::Program m_program;
    gl::VertexArray m_vertex_array;
    gl::Buffer m_vertex_buffer;
    gl::Buffer m_index_buffer;

    static constexpr std::array m_indices {
        0u, // top-left
        1u, // top-right
        2u, // bottom-left
        3u, // bottom-right
        2u, // bottom-left
        1u, // top-right
    };

public:
    explicit CircleRenderer(const gfx::Surface& surface);
    void draw(gfx::Vec center, float radius, gfx::Color color, glm::mat4 view);

};
