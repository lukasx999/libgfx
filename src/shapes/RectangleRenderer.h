#pragma once

#include <array>

#include <Window.h>
#include <types.h>
#include "../util.h"

class RectangleRenderer {
    const gfx::Window& m_window;

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
    explicit RectangleRenderer(const gfx::Window& window);
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
