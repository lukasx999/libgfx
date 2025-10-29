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
    Texture(int width, int height, int channels, unsigned char* bytes) {
        generate_opengl_texture(bytes, width, height, channels);
    }

    Texture(const Texture& other) {
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

    Texture(Texture&& other) = delete;

    Texture& operator=(const Texture& other) {
        Texture temp(other);
        std::swap(m_texture, temp.m_texture);
        return *this;
    }

    Texture& operator=(Texture&& other) = delete;

    ~Texture();

    [[nodiscard]] int get_width() const {
        int width;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glBindTexture(GL_TEXTURE_2D, 0);
        return width;
    }

    [[nodiscard]] int get_height() const {
        int height;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        glBindTexture(GL_TEXTURE_2D, 0);
        return height;
    }

    [[nodiscard]] int get_channels() const {
        int internal_format;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
        glBindTexture(GL_TEXTURE_2D, 0);
        return opengl_format_to_channels(internal_format);
    }

private:
    void load_texture_from_file(const char* path);
    void generate_opengl_texture(const unsigned char* data, int width, int height, int channels);

    [[nodiscard]] static constexpr
    GLint channels_to_opengl_format(int channels) {
        switch (channels) {
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
        }
        throw std::runtime_error("invalid channel count");
    }

    [[nodiscard]] static constexpr
    int opengl_format_to_channels(GLint format) {
        switch (format) {
            case GL_RGB: return 3;
            case GL_RGBA: return 4;
        }
        throw std::runtime_error("invalid texture format");
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
