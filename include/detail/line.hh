#pragma once

#include <array>

#include <window.hh>
#include <types.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class LineRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;

public:
    explicit LineRenderer(gfx::Window& window);
    void draw(float x0, float y0, float x1, float y1, gfx::Color color, glm::mat4 view);

};

} // namespace gfx::detail
