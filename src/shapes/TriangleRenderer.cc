#include <array>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "TriangleRenderer.h"
#include "../util.h"
#include "../shaders.h"

TriangleRenderer::TriangleRenderer(gfx::Window& window)
: m_window(window)
{

    m_program = create_shader_program(shaders::vertex::default_, shaders::fragment::default_);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleRenderer::draw(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    auto vertices = std::to_array<glm::vec2>({
        { x0, y0 },
        { x1, y1 },
        { x2, y2 },
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

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}
