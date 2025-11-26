#pragma once

#include <memory>

class TextureRenderer;

namespace gfx {

class Texture {
    friend TextureRenderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    // construct a texture from a file
    explicit Texture(const char* path);

    // construct a texture from a file
    explicit Texture(const std::string& path);

    // construct a texture from memory
    Texture(int width, int height, int channels, unsigned char* bytes);

    ~Texture();
    Texture(const Texture& other);
    Texture(Texture&& other);
    Texture& operator=(const Texture& other);
    Texture& operator=(Texture&& other);

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] int get_channels() const;

private:
    void load_texture_from_file(const char* path);
    void generate_texture(const unsigned char* data, int width, int height, int channels);

};

} // namespace gfx
