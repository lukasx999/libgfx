#pragma once

#include <memory>
#include <vector>
#include <string>

#include <gfx/rect.h>

class TextureRenderer;

namespace gfx {

class Renderer;

class Texture {
    friend TextureRenderer;
    friend gfx::Renderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    enum class Format { R, RG, RGB, RGBA };
    enum class FileType { Png, Bmp, Tga, Jpg };

    Texture();

    // construct a texture from a file
    explicit Texture(const char* path);
    explicit Texture(const std::string& path);

    // construct a texture from memory
    // passing nullptr to bytes will reserve memory only
    Texture(int width, int height, Format format=Format::RGBA, const unsigned char* bytes=nullptr);

    ~Texture();
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    Texture(Texture&&);
    Texture& operator=(Texture&&);

    // construct a new texture from a sub region of the current texture
    [[nodiscard]] Texture slice(gfx::Rect region) const;
    [[nodiscard]] Texture slice(float x, float y, float width, float height) const;

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] Format get_format() const;

    void write_to_file(FileType filetype, const char* filename) const;

private:
    void load_texture_from_file(const char* path);
    [[nodiscard]] std::vector<unsigned char> copy_to_cpu() const;
    [[nodiscard]] static int format_to_channels(Format format);
    [[nodiscard]] static Format channels_to_format(int channels);

};

} // namespace gfx
