#pragma once

#include <Window.h>
#include <Texture.h>
#include "../util.h"

class TextureRenderer {
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

    struct Vertex {
        glm::vec2 pos;
        glm::vec2 uv;
    };

public:
    explicit TextureRenderer(gfx::Window& window);

    void draw(float x, float y, float width, float height, const gfx::IRotation& rotation, const gfx::Texture& texture, glm::mat4 view);
    void draw_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, const gfx::IRotation& rotation, const gfx::Texture& texture, glm::mat4 view);

};
