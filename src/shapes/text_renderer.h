#pragma once

#include <memory>
#include <string_view>

#include <gfx/window.h>
#include <gfx/font.h>
#include <gfx/color.h>
#include "../opengl.h"
#include "../glyph.h"
#include "../font_impl.h"

class TextRenderer final {
    const gfx::Surface& m_surface;

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
    TextRenderer(const gfx::Surface& surface);

    void draw(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color, glm::mat4 view);

private:
    void draw_char(gfx::Vec pos, const Glyph& glyph, gfx::Color color, int fontsize, glm::mat4 view);

};

