#include "TextureImpl.h"

namespace gfx {

GLint Texture::Impl::channels_to_opengl_format(int channels) {
    switch (channels) {
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
    }
    throw gfx::Error("invalid channel count");
}

int Texture::Impl::opengl_format_to_channels(GLint format) {
    switch (format) {
        case GL_RGB: return 3;
        case GL_RGBA: return 4;
    }
    throw gfx::Error("invalid texture format");
}

gl::Texture Texture::Impl::generate_texture(const unsigned char* data, int width, int height, int channels) {

    gl::Texture texture;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto format = Impl::channels_to_opengl_format(channels);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

} // namespace gfx
