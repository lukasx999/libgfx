#include "text_renderer.h"
#include "../shaders.h"
#include "../font_impl.h"
#include "../texture_impl.h"

TextRenderer::TextRenderer(const gfx::Surface& surface) : m_surface(surface) {

    m_program = gl::create_shader_program(shaders::vertex::texture, shaders::fragment::text);

    glBindVertexArray(m_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec2), m_vertices.data(), GL_STATIC_DRAW);
    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_pos);

    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), m_uvs.data(), GL_STATIC_DRAW);
    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(a_uv);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}

void TextRenderer::draw(gfx::Vec pos, int fontsize, std::string_view text, const gfx::Font& font, gfx::Color color, gfx::Rotation rotation, glm::mat4 view) {

    auto [x, y] = pos;
    int offset = 0;

    for (auto c : text) {
        auto glyph = font.m_pimpl->load_glyph(c, fontsize);
        draw_char({ x+offset, y }, glyph, color, fontsize, rotation, view);
        offset += glyph.m_advance_x;
    }

}

void TextRenderer::draw_char(gfx::Vec pos, const Glyph& glyph, gfx::Color color, int fontsize, gfx::Rotation rotation, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    auto [x, y] = pos;

    int width = glyph.m_texture.get_width();
    int height = glyph.m_texture.get_height();

    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(x+width/2.0, y+height/2.0, 0.0));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-x-width/2.0, -y-height/2.0, 0.0));

    model = glm::translate(model, glm::vec3(x + glyph.m_bearing_x, y - glyph.m_bearing_y + fontsize, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 0.0f));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_surface.get_width()),
        static_cast<float>(m_surface.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;
    gl::set_uniform(m_program, "u_mvp", mvp);

    GLint u_color = glGetUniformLocation(m_program, "u_color");
    auto c = color.normalized();
    glUniform4f(u_color, c.r, c.g, c.b, c.a);

    glBindTexture(GL_TEXTURE_2D, glyph.m_texture.m_pimpl->m_texture);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}
