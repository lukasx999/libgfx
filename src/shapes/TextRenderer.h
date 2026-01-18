#pragma once

#include <memory>

#include <Window.h>
#include <Font.h>
#include <Color.h>
#include "../opengl.h"
#include "../Glyph.h"
#include "../FontImpl.h"

class TextRenderer final {
    const gfx::Window& m_window;

    gl::Program m_program;
    gl::VertexArray m_vertex_array;
    gl::Buffer m_vertex_buffer;
    gl::Buffer m_uv_buffer;
    gl::Buffer m_index_buffer;

    static constexpr std::array m_vertices {
        glm::vec2(1.0, 1.0), // top-right
        glm::vec2(0.0, 1.0), // top-left
        glm::vec2(0.0, 0.0), // bottom-left
        glm::vec2(1.0, 0.0), // bottom-right
    };

    static constexpr std::array m_uvs {
        glm::vec2(1.0, 1.0), // top-right
        glm::vec2(0.0, 1.0), // top-left
        glm::vec2(0.0, 0.0), // bottom-left
        glm::vec2(1.0, 0.0), // bottom-right
    };

    static constexpr std::array m_indices {
        0u, 1u, 2u,
        3u, 2u, 0u,
    };

public:
    TextRenderer(const gfx::Window& window);

    void draw(float x, float y, int text_size, const char* text, const gfx::Font& font, gfx::Color color, glm::mat4 view);

private:
    void draw_char(float x, float y, const Glyph& glyph, gfx::Color color, int text_size, glm::mat4 view);

};

