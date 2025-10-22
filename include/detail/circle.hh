#pragma once

#include <vector>

#include <types.hh>
#include <window.hh>
#include <detail/detail.hh>

namespace gfx::detail {

class CircleRenderer : public IDeferredRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    struct Vertex {
        glm::vec2 pos;
        glm::vec4 color;
        glm::mat4 transform;
        float radius;
        glm::vec2 center;
    };

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

public:
    explicit CircleRenderer(gfx::Window& window);

    void draw(float x, float y, float radius, gfx::Color color, glm::mat4 view);
    void flush() override;

};

} // namespace gfx::detail
