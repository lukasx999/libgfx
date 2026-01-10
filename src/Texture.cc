#include <utility>
#include <memory>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Texture.h>
#include "TextureImpl.h"
#include "util.h"

namespace gfx {

Texture::Texture() : m_pimpl(std::make_unique<Texture::Impl>()) {
    if (!library_has_been_initialized)
        throw gfx::Error("cannot construct a gfx::Texture before the gfx library has been initialized. construct a gfx::Window to fix this issue.");
}

Texture::Texture(const char* path) : Texture() {
    load_texture_from_file(path);
}

Texture::Texture(const std::string& path) : Texture() {
    load_texture_from_file(path.c_str());
}

Texture::Texture(int width, int height, int channels, unsigned char* bytes) : Texture() {
    m_pimpl->m_texture = Impl::generate_texture(bytes, width, height, channels);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_pimpl->m_texture);
}

// there's no need for delegating to the default constructor to check if the library
// has been initialized in the copy/move ctor, because there's no (reasonable) way to call these ctors
// without already having another gfx::Texture

Texture::Texture(const Texture& other) : m_pimpl(std::make_unique<Texture::Impl>()) {
    int width = other.get_width();
    int height = other.get_height();
    int channels = other.get_channels();
    GLint format = Impl::channels_to_opengl_format(channels);

    std::vector<unsigned char> buf(width * height * channels);
    glBindTexture(GL_TEXTURE_2D, other.m_pimpl->m_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, buf.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    m_pimpl->m_texture = Impl::generate_texture(buf.data(), width, height, channels);
}

Texture::Texture(Texture&& other)
: m_pimpl(std::make_unique<Texture::Impl>(std::exchange(other.m_pimpl->m_texture, 0)))
{ }

Texture& Texture::operator=(const Texture& other) {
    Texture temp(other);
    std::swap(m_pimpl->m_texture, temp.m_pimpl->m_texture);
    return *this;
}

Texture& Texture::operator=(Texture&& other) {
    std::swap(m_pimpl->m_texture, other.m_pimpl->m_texture);
    return *this;
}

Texture Texture::slice(gfx::Rect region) const {
    return slice(region.x, region.y, region.width, region.height);
}

Texture Texture::slice(int x, int y, int width, int height) const {

    int channels = get_channels();
    GLint format = Impl::channels_to_opengl_format(channels);

    std::vector<unsigned char> buf(width * height * channels);
    glGetTextureSubImage(m_pimpl->m_texture, 0, x, y, 0, width, height, 1, format, GL_UNSIGNED_BYTE, buf.size() * sizeof(unsigned char), buf.data());

    return Texture(width, height, channels, buf.data());
}

void Texture::load_texture_from_file(const char* path) {

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == nullptr)
        throw gfx::Error(std::format("failed to load texture: {}", stbi_failure_reason()));

    m_pimpl->m_texture = Impl::generate_texture(data, width, height, channels);
    stbi_image_free(data);
}

int Texture::get_width() const {
    int width;
    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glBindTexture(GL_TEXTURE_2D, 0);
    return width;
}

int Texture::get_height() const {
    int height;
    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glBindTexture(GL_TEXTURE_2D, 0);
    return height;
}

int Texture::get_channels() const {
    int internal_format;
    glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
    glBindTexture(GL_TEXTURE_2D, 0);
    return Impl::opengl_format_to_channels(internal_format);
}

} // namespace gfx
