#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "RectangleRenderer.h"
#include "../shaders.h"

RectangleRenderer::RectangleRenderer(const gfx::Window& window)
: m_window(window)
{

    m_program = create_shader_program(shaders::vertex::default_, shaders::fragment::default_);

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

void RectangleRenderer::draw(gfx::Rect dest, gfx::Rotation rotation, gfx::Color color, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    auto [x, y, width, height] = dest;

    auto vertices = std::to_array<glm::vec2>({
        { x,       y        }, // top-left
        { x+width, y        }, // top-right
        { x,       y+height }, // bottom-left
        { x+width, y+height }, // bottom-right
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

    // the translations are needed to subtract the world space coordinates (x,y)
    // since we want the rectangle to rotate around its top left corner, and
    // to set the center of rotation to the middle of the rectangle
    glm::mat4 model(1.0);
    model = glm::translate(model, glm::vec3(x+width/2.0, y+height/2.0, 0.0));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-x-width/2.0, -y-height/2.0, 0.0));

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

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

}
