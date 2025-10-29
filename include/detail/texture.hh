#pragma once

#include <unordered_map>
#include <vector>

#include <window.hh>
#include <detail/detail.hh>

namespace gfx {

namespace detail {
class TextureRenderer;
} // namespace detail

// WARNING: do not use this class before constructing a gfx::Window!
class Texture {
    friend detail::TextureRenderer;

    GLuint m_texture;
    int m_width;
    int m_height;
    int m_channels;

public:
    // construct a texture from a file
    explicit Texture(const char* path) {
        load_texture_from_file(path);
    }

    // construct a texture from a file
    explicit Texture(const std::string& path) {
        load_texture_from_file(path.c_str());
    }

    // construct a texture from memory
    Texture(int width, int height, int channels, unsigned char* bytes)
        : m_width(width)
        , m_height(height)
        , m_channels(channels)

    {
        generate_opengl_texture(bytes);
    }

    Texture(const Texture& other)
        : m_width(other.m_width)
        , m_height(other.m_height)
        , m_channels(other.m_channels)
    {
        auto format = get_opengl_texture_format();
        unsigned char* buf = new unsigned char[m_width * m_height * m_channels];
        glBindTexture(GL_TEXTURE_2D, other.m_texture);
        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, buf);
        glBindTexture(GL_TEXTURE_2D, 0);
        generate_opengl_texture(buf);
        delete[] buf;
    }

    Texture(Texture&& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) = delete;

    ~Texture();

    [[nodiscard]] int get_width() const {
        return m_width;
    }

    [[nodiscard]] int get_height() const {
        return m_height;
    }

    [[nodiscard]] int get_channels() const {
        return m_channels;
    }

private:
    void load_texture_from_file(const char* path);
    void generate_opengl_texture(const unsigned char* data);

    [[nodiscard]] constexpr GLint get_opengl_texture_format() const {
        switch (m_channels) {
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
        }
        throw std::runtime_error("invalid channel count");
    }

};
} // namespace gfx

namespace gfx::detail {

class TextureRenderer {
    gfx::Window& m_window;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_index_buffer;

    static constexpr std::array m_indices {
        0u, // top-left
        1u, // top-right
        2u, // bottom-left
        3u, // bottom-right
        2u, // bottom-left
        1u, // top-right
    };

    struct Vertex {
        glm::vec2 pos;
        glm::vec2 uv;
    };

public:
    explicit TextureRenderer(gfx::Window& window);

    void draw(
        float x,
        float y,
        float width,
        float height,
        const gfx::IRotation& rotation,
        const gfx::Texture& texture,
        glm::mat4 view
    );

    void draw_sub(
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
    );

};

} // namespace gfx::detail
