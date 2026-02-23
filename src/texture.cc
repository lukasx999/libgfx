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
#include <gfx/renderer.h>
#include <gfx/external_context.h>

namespace gfx {

Texture::Texture() : Texture(0, 0) { }

Texture::Texture(const char* path) : m_pimpl(std::make_unique<Texture::Impl>()) {
    load_texture_from_file(path);
}

Texture::Texture(const std::string& path) : m_pimpl(std::make_unique<Texture::Impl>()) {
    load_texture_from_file(path.c_str());
}

Texture::Texture(int width, int height, Format format, const unsigned char* bytes)
    : m_pimpl(std::make_unique<Texture::Impl>())
    , m_width(width)
    , m_height(height)
    , m_format(format)
{
    m_pimpl->m_texture = Impl::generate_texture(width, height, format, bytes);
}

// the pimpl pattern requires the destructor to "see" the complete
// type of the Impl structure
Texture::~Texture() = default;
Texture::Texture(Texture&&) = default;
Texture& Texture::operator=(Texture&&) = default;

Texture::Texture(const Texture& other) : Texture(other.m_width, other.m_height, other.m_format) {
    // not using glGetTexImage() as its not supported by GLES3
    // also doing it this way is much faster, because glGetTexImage() copies the texture to the cpu

    gl::Framebuffer framebuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // we have to save the old viewport size, because when we draw into the newly created
    // framebuffer, the vertices will get transformed from NDC to the old(!) viewport coordinates.
    // the window is probably bigger than the framebuffer, which will result in the scaled coordinates
    // getting clipped.
    std::array<GLint, 4> saved_viewport;
    glGetIntegerv(GL_VIEWPORT, saved_viewport.data());
    glViewport(0, 0, m_width, m_height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pimpl->m_texture, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    gfx::ExternalContext ctx(m_width, m_height);
    gfx::Renderer rd(ctx);
    rd.draw_texture(ctx.get_as_rect(), other);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(saved_viewport[0], saved_viewport[1], saved_viewport[2], saved_viewport[3]);
}

Texture& Texture::operator=(const Texture& other) {
    Texture temp(other);
    std::swap(m_pimpl->m_texture, temp.m_pimpl->m_texture);
    m_width = other.m_width;
    m_height = other.m_height;
    m_format = other.m_format;
    return *this;
}

Texture Texture::slice(gfx::Rect region) const {
    // not using glGetTextureSubImage() as its not supported by GLES3
    // also doing it this way is much faster, because glGetTextureSubImage() copies the texture to the cpu

    auto [x, y, width, height] = region;

    gfx::Texture texture(width, height, m_format);

    gl::Framebuffer framebuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    std::array<GLint, 4> saved_viewport;
    glGetIntegerv(GL_VIEWPORT, saved_viewport.data());
    glViewport(0, 0, width, height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_pimpl->m_texture, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    gfx::ExternalContext ctx(width, height);
    gfx::Renderer rd(ctx);
    rd.draw_texture_sub(ctx.get_as_rect(), region, *this);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(saved_viewport[0], saved_viewport[1], saved_viewport[2], saved_viewport[3]);

    return texture;
}

Texture Texture::slice(float x, float y, float width, float height) const {
    return slice({ x, y, width, height });
}

void Texture::load_texture_from_file(const char* path) {

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == nullptr)
        throw gfx::Error(std::format("failed to load texture: {}", stbi_failure_reason()));

    Format format = channels_to_format(channels);
    m_width = width;
    m_height = height;
    m_format = format;
    m_pimpl->m_texture = Impl::generate_texture(width, height, format, data);
    stbi_image_free(data);
}

int Texture::get_width() const {
    // not using glGetTexLevelParameteriv() as its not supported by GLES3
    return m_width;
}

int Texture::get_height() const {
    // not using glGetTexLevelParameteriv() as its not supported by GLES3
    return m_height;
}

Texture::Format Texture::get_format() const {
    // not using glGetTexLevelParameteriv() as its not supported by GLES3
    return m_format;
}

void Texture::write_to_file(Filetype filetype, const char* filename) const {

    // TODO: glReadPixels() on gles3
    throw gfx::Error("unimplemented");

    // int width = get_width();
    // int height = get_height();
    // auto buf = copy_to_cpu();
    // int channels = format_to_channels(get_format());
    //
    // stbi_flip_vertically_on_write(true);
    //
    // int ret = [&] {
    //     switch (filetype) {
    //         using enum Filetype;
    //         case Png: return stbi_write_png(filename, width, height, channels, buf.data(), 0);
    //         case Bmp: return stbi_write_bmp(filename, width, height, channels, buf.data());
    //         case Tga: return stbi_write_tga(filename, width, height, channels, buf.data());
    //         case Jpg: return stbi_write_jpg(filename, width, height, channels, buf.data(), 100);
    //     }
    // }();
    //
    // if (ret == 0)
    //     throw gfx::Error("failed to write texture to file");

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
