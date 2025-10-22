#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <detail/circle.hh>
#include "shaders.hh"

namespace gfx::detail {

CircleRenderer::CircleRenderer(gfx::Window& window)
: m_window(window)
{
    m_program = create_shader_program(shaders::vertex::circle, shaders::fragment::circle);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_index_buffer);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(a_pos);

    GLint a_center = glGetAttribLocation(m_program, "a_center");
    glVertexAttribPointer(a_center, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, center)));
    glEnableVertexAttribArray(a_center);

    GLint a_radius = glGetAttribLocation(m_program, "a_radius");
    glVertexAttribPointer(a_radius, 1, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, radius)));
    glEnableVertexAttribArray(a_radius);

    GLint a_color = glGetAttribLocation(m_program, "a_color");
    glVertexAttribPointer(a_color, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(a_color);

    GLint a_mvp = glGetAttribLocation(m_program, "a_mvp");
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(
            a_mvp+i,
            4,
            GL_FLOAT,
            false,
            sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, transform) + i * sizeof(glm::vec4))
        );
        glEnableVertexAttribArray(a_mvp+i);
    }

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CircleRenderer::draw(float x, float y, float radius, gfx::Color color, glm::mat4 view) {

    int vertex_count =  m_vertices.size();
    m_indices.push_back(0u + vertex_count); // top-left
    m_indices.push_back(1u + vertex_count); // top-right
    m_indices.push_back(2u + vertex_count); // bottom-left
    m_indices.push_back(3u + vertex_count); // bottom-right
    m_indices.push_back(2u + vertex_count); // bottom-left
    m_indices.push_back(1u + vertex_count); // top-right

    Vertex v1, v2, v3, v4;

    v1.pos = { x - radius, y - radius }; // top-left
    v2.pos = { x + radius, y - radius }; // top-right
    v3.pos = { x - radius, y + radius }; // bottom-left
    v4.pos = { x + radius, y + radius }; // bottom-right

    glm::vec2 center = view * glm::vec4(x, y, 0.0f, 1.0f);

    v1.center = center;
    v2.center = center;
    v3.center = center;
    v4.center = center;

    v1.radius = radius;
    v2.radius = radius;
    v3.radius = radius;
    v4.radius = radius;

    glm::mat4 model(1.0);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    v1.transform = mvp;
    v2.transform = mvp;
    v3.transform = mvp;
    v4.transform = mvp;

    auto c = color.normalized();
    glm::vec4 cv(c.r, c.g, c.b, c.a);

    v1.color = cv;
    v2.color = cv;
    v3.color = cv;
    v4.color = cv;

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
    m_vertices.push_back(v4);

}

void CircleRenderer::flush() {

    glBindVertexArray(m_vertex_array);
    glUseProgram(m_program);

    GLint u_window_height = glGetUniformLocation(m_program, "u_window_height");
    glUniform1i(u_window_height, m_window.get_height());

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    m_vertices.clear();
    m_indices.clear();
}

} // namespace gfx::detail
