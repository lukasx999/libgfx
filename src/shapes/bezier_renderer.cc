#include "../shaders.h"
#include "bezier_renderer.h"
#include <glm/glm.hpp>

BezierRenderer::BezierRenderer(const gfx::Surface& surface) : m_surface(surface) {

    m_program = gl::create_shader_program(shaders::vertex::default_, shaders::fragment::bezier);

    glBindVertexArray(m_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);
}

void BezierRenderer::draw(gfx::Vec a, gfx::Vec b, gfx::Vec control, gfx::Color color, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    auto vertices = std::to_array<glm::vec2>({
        glm::vec2(a.x, a.y),
        glm::vec2(b.x, b.y),
        glm::vec2(control.x, control.y),
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

    glm::mat4 model(1.0);

    auto projection = gl::get_surface_projection(m_surface);

    glm::mat4 mvp = projection * view * model;
    gl::set_uniform(m_program, "u_mvp", mvp);

    auto col = color.normalized();
    GLint u_color = glGetUniformLocation(m_program, "u_color");
    glUniform4f(u_color, col.r, col.g, col.b, col.a);

    GLint u_p1 = glGetUniformLocation(m_program, "u_p1");
    glUniform2f(u_p1, a.x, a.y);

    GLint u_p2 = glGetUniformLocation(m_program, "u_p2");
    glUniform2f(u_p2, b.x, b.y);

    GLint u_control = glGetUniformLocation(m_program, "u_control");
    glUniform2f(u_control, control.x, control.y);

    gl::set_uniform(m_program, "u_window_height", m_surface.get_height());

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
