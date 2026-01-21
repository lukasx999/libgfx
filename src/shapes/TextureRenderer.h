#pragma once

#include <Window.h>
#include <Texture.h>
#include <Color.h>
#include "../opengl.h"

class TextureRenderer final {
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

    struct Vertex {
        glm::vec2 pos;
        glm::vec2 uv;
    };

public:
    explicit TextureRenderer(const gfx::Surface& surface);

    void draw(float x, float y, float width, float height, gfx::Rotation rotation, const gfx::Texture& texture, glm::mat4 view);
    void draw_sub(float dest_x, float dest_y, float dest_width, float dest_height, float src_x, float src_y, float src_width, float src_height, gfx::Rotation rotation, const gfx::Texture& texture, glm::mat4 view);

};
