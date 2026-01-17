#pragma once

#include <memory>
#include <vector>

#include "Rect.h"

class TextureRenderer;

namespace gfx {
class Renderer;
} // namespace gfx

namespace gfx {

class Texture {
    friend TextureRenderer;
    friend gfx::Renderer;
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    Texture();

public:
    // construct a texture from a file
    explicit Texture(const char* path);

    // construct a texture from a file
    explicit Texture(const std::string& path);

    // construct a texture from memory
    // passing nullptr to bytes will reserve memory only
    Texture(int width, int height, int channels, unsigned char* bytes);

    ~Texture();
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    Texture(Texture&&);
    Texture& operator=(Texture&&);

    // construct a new texture from a sub region of the current texture
    [[nodiscard]] Texture slice(int x, int y, int width, int height) const;
    [[nodiscard]] Texture slice(gfx::Rect region) const;

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] int get_channels() const;

    enum class FileType { Png, Bmp, Tga, Jpg };

    void write_to_file(FileType filetype, const char* filename) const;

private:
    void load_texture_from_file(const char* path);
    [[nodiscard]] std::vector<unsigned char> copy_to_cpu() const;

};

} // namespace gfx
