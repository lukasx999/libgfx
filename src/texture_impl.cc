#include <gfx/error.h>
#include "texture_impl.h"

namespace gfx {

GLint Texture::Impl::gfx_format_to_opengl_format(Format format) {
    switch (format) {
        using enum Format;
        case R: return GL_RED;
        case RG: return GL_RG;
        case RGB: return GL_RGB;
        case RGBA: return GL_RGBA;
    }
    std::unreachable();
}

Texture::Format Texture::Impl::opengl_format_to_gfx_format(GLint format) {
    switch (format) {
        case GL_RED: return Format::R;
        case GL_RG: return Format::RG;
        case GL_RGB: return Format::RGB;
        case GL_RGBA: return Format::RGBA;
    }
    throw gfx::Error("unknown texture format");
}

gl::Texture Texture::Impl::generate_texture(int width, int height, const unsigned char* data, Format format) {

    gl::Texture texture;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint gl_format = Impl::gfx_format_to_opengl_format(format);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

} // namespace gfx
