#include <utility>

#include "texture_renderer.h"
#include "../shaders.h"
#include "../opengl.h"
#include "../texture_impl.h"

TextureRenderer::TextureRenderer(const gfx::Surface& surface) : m_surface(surface) {

    m_program = gl::create_shader_program(shaders::vertex::texture, shaders::fragment::texture);

    glBindVertexArray(m_vertex_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

    GLint a_pos = glGetAttribLocation(m_program, "a_pos");
    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(a_pos);

    GLint a_uv = glGetAttribLocation(m_program, "a_uv");
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(a_uv);

    // just to make sure everything still works after unbinding, as other classes/functions may
    // modify opengl state after running the ctor
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextureRenderer::draw(gfx::Rect rect, const gfx::Texture& texture, gfx::Rotation rotation, glm::mat4 view) {
    draw_sub(rect, { 0.0f, 0.0f, static_cast<float>(texture.get_width()), static_cast<float>(texture.get_height()) }, texture, rotation, view);
}

void TextureRenderer::draw_sub(gfx::Rect dest, gfx::Rect src, const gfx::Texture& texture, gfx::Rotation rotation, glm::mat4 view) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    int tex_width = texture.get_width();
    int tex_height = texture.get_height();

    glm::vec2 uv_start {
        src.x / tex_width,
        src.y / tex_height,
    };

    glm::vec2 uv_end {
        (src.x + src.width) / tex_width,
        (src.y + src.height) / tex_height,
    };

    auto vertices = std::to_array<Vertex>({
        { { dest.x,            dest.y             }, { uv_start.x, uv_start.y } }, // top-left
        { { dest.x+dest.width, dest.y             }, { uv_end.x,   uv_start.y } }, // top-right
        { { dest.x,            dest.y+dest.height }, { uv_start.x, uv_end.y   } }, // bottom-left
        { { dest.x+dest.width, dest.y+dest.height }, { uv_end.x,   uv_end.y   } }, // bottom-right
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    // the translations are needed to subtract the world space coordinates (x,y)
    // since we want the rectangle to rotate around its top left corner, and
    // to set the center of rotation to the middle of the rectangle
    glm::mat4 model(1.0);
    model = glm::translate(model, glm::vec3(dest.x+dest.width/2.0, dest.y+dest.height/2.0, 0.0));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-dest.x-dest.width/2.0, -dest.y-dest.height/2.0, 0.0));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_surface.get_width()),
        static_cast<float>(m_surface.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;
    gl::set_uniform(m_program, "u_mvp", mvp);

    glBindTexture(GL_TEXTURE_2D, texture.m_pimpl->m_texture);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}
