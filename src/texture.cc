#include <utility>
#include <memory>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

// stb_image_write.h uses c-style initializers for their structs
// eg: `Struct struct = { 0 }`, which yields a warning when compiling as C++
// when not all members are mentioned
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <stb_image_write.h>
#pragma GCC diagnostic pop

#include <gfx/texture.h>
#include <gfx/error.h>
#include "texture_impl.h"
#include "opengl.h"

namespace gfx {

Texture::Texture() : Texture(0, 0) { }

Texture::Texture(const char* path) : m_pimpl(std::make_unique<Texture::Impl>()) {
    load_texture_from_file(path);
}

Texture::Texture(const std::string& path) : m_pimpl(std::make_unique<Texture::Impl>()) {
    load_texture_from_file(path.c_str());
}

Texture::Texture(int width, int height, Format format, const unsigned char* bytes) : m_pimpl(std::make_unique<Texture::Impl>()) {
    m_pimpl->m_texture = Impl::generate_texture(width, height, bytes, format);
}

// the pimpl pattern requires the destructor to "see" the complete
// type of the Impl structure
Texture::~Texture() = default;
Texture::Texture(Texture&&) = default;
Texture& Texture::operator=(Texture&&) = default;

Texture::Texture(const Texture& other) : m_pimpl(std::make_unique<Texture::Impl>()) {
    auto buf = other.copy_to_cpu();
    m_pimpl->m_texture = Impl::generate_texture(other.get_width(), other.get_height(), buf.data(), other.get_format());
}

Texture& Texture::operator=(const Texture& other) {
    Texture temp(other);
    std::swap(m_pimpl->m_texture, temp.m_pimpl->m_texture);
    return *this;
}

Texture Texture::slice(gfx::Rect region) const {
    auto format = get_format();
    GLint gl_format = Impl::gfx_format_to_opengl_format(format);
    int channels = format_to_channels(format);

    auto [x, y, width, height] = region;

    std::vector<unsigned char> buf(width * height * channels);
#ifdef __EMSCRIPTEN__
    throw gfx::Error("copying textures in unimplemented on web platform");
#else
    glGetTextureSubImage(m_pimpl->m_texture, 0, x, y, 0, width, height, 1, gl_format, GL_UNSIGNED_BYTE, buf.size() * sizeof(unsigned char), buf.data());
#endif // __EMSCRIPTEN__

    return Texture(width, height, format, buf.data());
}

Texture Texture::slice(float x, float y, float width, float height) const {
    return slice({ x, y, width, height });
}

void Texture::load_texture_from_file(const char* path) {

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == nullptr)
        throw gfx::Error(std::format("failed to load texture: {}", stbi_failure_reason()));

    m_pimpl->m_texture = Impl::generate_texture(width, height, data, channels_to_format(channels));
    stbi_image_free(data);
}

int Texture::get_width() const {
    int width;
    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
#ifdef __EMSCRIPTEN__
    throw gfx::Error("retrieving texture width is unimplemented on web platform");
#else
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
#endif // __EMSCRIPTEN__
    glBindTexture(GL_TEXTURE_2D, 0);
    return width;
}

int Texture::get_height() const {
    int height;
    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
#ifdef __EMSCRIPTEN__
    throw gfx::Error("retrieving texture width is unimplemented on web platform");
#else
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
#endif // __EMSCRIPTEN__
    glBindTexture(GL_TEXTURE_2D, 0);
    return height;
}

Texture::Format Texture::get_format() const {
    int internal_format;
    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
#ifdef __EMSCRIPTEN__
    throw gfx::Error("retrieving texture format is unimplemented on web platform");
#else
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
#endif // __EMSCRIPTEN__
    glBindTexture(GL_TEXTURE_2D, 0);
    return Impl::opengl_format_to_gfx_format(internal_format);
}

void Texture::write_to_file(Filetype filetype, const char* filename) const {

    int width = get_width();
    int height = get_height();
    auto buf = copy_to_cpu();
    int channels = format_to_channels(get_format());

    stbi_flip_vertically_on_write(true);

    int ret = [&] {
        switch (filetype) {
            using enum Filetype;
            case Png: return stbi_write_png(filename, width, height, channels, buf.data(), 0);
            case Bmp: return stbi_write_bmp(filename, width, height, channels, buf.data());
            case Tga: return stbi_write_tga(filename, width, height, channels, buf.data());
            case Jpg: return stbi_write_jpg(filename, width, height, channels, buf.data(), 100);
        }
    }();

    if (ret == 0)
        throw gfx::Error("failed to write texture to file");

}

std::vector<unsigned char> Texture::copy_to_cpu() const {
    GLint format = Impl::gfx_format_to_opengl_format(get_format());

    int channels = format_to_channels(get_format());
    std::vector<unsigned char> buf(get_width() * get_height() * channels);

    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
#ifdef __EMSCRIPTEN__
    throw gfx::Error("copying textures is unimplemented on web platform");
#else
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, buf.data());
#endif // __EMSCRIPTEN__
    glBindTexture(GL_TEXTURE_2D, 0);

    return buf;
}

int Texture::format_to_channels(Format format) {
    switch (format) {
        using enum Format;
        case R:    return 1;
        case RG:   return 2;
        case RGB:  return 3;
        case RGBA: return 4;
    }
    std::unreachable();
}

Texture::Format Texture::channels_to_format(int channels) {
    switch (channels) {
        using enum Format;
        case 1: return R;
        case 2: return RG;
        case 3: return RGB;
        case 4: return RGBA;
    }
    std::unreachable();
}

} // namespace gfx
