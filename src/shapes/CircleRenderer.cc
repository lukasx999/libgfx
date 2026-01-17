#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "CircleRenderer.h"
#include "../shaders.h"

CircleRenderer::CircleRenderer(const gfx::Window& window)
: m_window(window)
{
    m_program = create_shader_program(shaders::vertex::default_, shaders::fragment::circle);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CircleRenderer::draw(gfx::Vec center, float radius, gfx::Color color, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    auto [x, y] = center;

    auto vertices = std::to_array<glm::vec2>({
        { x - radius, y - radius }, // top-left
        { x + radius, y - radius }, // top-right
        { x - radius, y + radius }, // bottom-left
        { x + radius, y + radius }, // bottom-right
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

    glm::mat4 model(1.0);

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

    auto c = color.normalized();
    GLint u_color = glGetUniformLocation(m_program, "u_color");
    glUniform4f(u_color, c.r, c.g, c.b, c.a);

    GLint u_center = glGetUniformLocation(m_program, "u_center");
    auto view_center = view * glm::vec4(x, y, 0.0, 1.0);
    glUniform2f(u_center, view_center.x, view_center.y);

    GLint u_radius = glGetUniformLocation(m_program, "u_radius");
    glUniform1f(u_radius, radius);

    GLint u_window_height = glGetUniformLocation(m_program, "u_window_height");
    glUniform1i(u_window_height, m_window.get_height());

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

}

