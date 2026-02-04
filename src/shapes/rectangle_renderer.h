#pragma once

#include <array>

#include <gfx/window.h>
#include <gfx/rotation.h>
#include <gfx/color.h>
#include <gfx/rect.h>
#include "../opengl.h"

class RectangleRenderer final {
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
    explicit RectangleRenderer(const gfx::Surface& surface);
    void draw(gfx::Rect rect, gfx::Color color, gfx::Rotation rotation, glm::mat4 view);

};
