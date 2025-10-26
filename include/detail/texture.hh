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
    unsigned char* m_data;

public:
    explicit Texture(const char* path);
    explicit Texture(const std::string& path);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

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
    void load_texture(const char* path);

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

};

} // namespace gfx::detail
