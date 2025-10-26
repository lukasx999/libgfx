#pragma once

#include <vector>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class RectangleRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_indices {
        0u, // top-left
        1u, // top-right
        2u, // bottom-left
        3u, // bottom-right
        2u, // bottom-left
        1u, // top-right
    };

public:
    explicit RectangleRenderer(gfx::Window& window);
    void draw(
        float x,
        float y,
        float width,
        float height,
        const gfx::IRotation& rotation,
        gfx::Color color,
        glm::mat4 view
    );

};

} // namespace gfx::detail
