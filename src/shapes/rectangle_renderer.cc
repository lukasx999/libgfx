#include "rectangle_renderer.h"
#include "../shaders.h"

RectangleRenderer::RectangleRenderer(const gfx::Surface& surface) : m_surface(surface) {

    m_program = gl::create_shader_program(shaders::vertex::default_, shaders::fragment::default_);

    glBindVertexArray(m_vertex_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

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

void RectangleRenderer::draw(gfx::Rect rect, gfx::Color color, gfx::Rotation rotation, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    auto [x, y, width, height] = rect;

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

    auto projection = gl::get_surface_projection(m_surface);

    glm::mat4 mvp = projection * view * model;
    gl::set_uniform(m_program, "u_mvp", mvp);

    auto c = color.normalized();
    GLint u_color = glGetUniformLocation(m_program, "u_color");
    glUniform4f(u_color, c.r, c.g, c.b, c.a);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

}
