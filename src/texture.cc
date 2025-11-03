#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <detail/texture.hh>
#include "shaders.hh"

namespace gfx {

void Texture::load_texture_from_file(const char* path) {

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == nullptr)
        throw std::runtime_error(std::format("failed to load texture: {}", stbi_failure_reason()));

    generate_opengl_texture(data, width, height, channels);
    stbi_image_free(data);
}

void Texture::generate_opengl_texture(const unsigned char* data, int width, int height, int channels) {

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto format = channels_to_opengl_format(channels);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const Texture& other) {
    int width = other.get_width();
    int height = other.get_height();
    int channels = other.get_channels();
    auto format = channels_to_opengl_format(channels);

    unsigned char* buf = new unsigned char[width * height * channels];
    glBindTexture(GL_TEXTURE_2D, other.m_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, buf);
    glBindTexture(GL_TEXTURE_2D, 0);
    generate_opengl_texture(buf, width, height, channels);
    delete[] buf;
}

Texture::~Texture() {
    glDeleteTextures(1, &m_texture);
}

namespace detail {

TextureRenderer::TextureRenderer(gfx::Window& window)
: m_window(window)
{
    m_program = create_shader_program(shaders::vertex::texture, shaders::fragment::texture);

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_vertex_buffer);
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

void TextureRenderer::draw(
    float x,
    float y,
    float width,
    float height,
    const gfx::IRotation& rotation,
    const gfx::Texture& texture,
    glm::mat4 view
) {
    draw_sub(x, y, width, height, 0, 0, texture.get_width(), texture.get_height(), rotation, texture, view);
}

void TextureRenderer::draw_sub(
    float dest_x,
    float dest_y,
    float dest_width,
    float dest_height,
    float src_x,
    float src_y,
    float src_width,
    float src_height,
    const gfx::IRotation& rotation,
    const gfx::Texture& texture,
    glm::mat4 view
) {

    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);

    int tex_width = texture.get_width();
    int tex_height = texture.get_height();

    glm::vec2 uv_start {
        src_x / tex_width,
        src_y / tex_height,
    };

    glm::vec2 uv_end {
        (src_x + src_width) / tex_width,
        (src_y + src_height) / tex_height,
    };

    auto vertices = std::to_array<Vertex>({
        { { dest_x,            dest_y             }, { uv_start.x, uv_start.y } }, // top-left
        { { dest_x+dest_width, dest_y             }, { uv_end.x,   uv_start.y } }, // top-right
        { { dest_x,            dest_y+dest_height }, { uv_start.x, uv_end.y   } }, // bottom-left
        { { dest_x+dest_width, dest_y+dest_height }, { uv_end.x,   uv_end.y   } }, // bottom-right
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    // the translations are needed to subtract the world space coordinates (x,y)
    // since we want the rectangle to rotate around its top left corner, and
    // to set the center of rotation to the middle of the rectangle
    glm::mat4 model(1.0);
    model = glm::translate(model, glm::vec3(dest_x+dest_width/2.0, dest_y+dest_height/2.0, 0.0));
    model = glm::rotate(model, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-dest_x-dest_width/2.0, -dest_y-dest_height/2.0, 0.0));

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(m_window.get_width()),
        static_cast<float>(m_window.get_height()),
        0.0f
    );

    glm::mat4 mvp = projection * view * model;

    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");
    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));

    glBindTexture(GL_TEXTURE_2D, texture.m_texture);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

} // namespace detail

} // namespace gfx
