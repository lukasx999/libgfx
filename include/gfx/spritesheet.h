#pragma once

#include <vector>

#include <gfx/texture.h>

namespace gfx {

class Spritesheet {
    const gfx::Texture& m_texture;
    const int m_sprite_width;
    const int m_sprite_height;

public:
    Spritesheet(const gfx::Texture& texture, int cell_width, int cell_height)
        : m_texture(texture)
        , m_sprite_width(cell_width)
        , m_sprite_height(cell_height)
    { }

    [[nodiscard]] int get_cell_width() const {
        return m_sprite_width;
    }

    [[nodiscard]] int get_cell_height() const {
        return m_sprite_height;
    }

    [[nodiscard]] std::vector<gfx::Texture> get_row(int y, int n) const {
        std::vector<gfx::Texture> row;

        for (int x = 0; x < n; ++x)
            row.push_back(get(x, y));

        return row;
    }

    [[nodiscard]] gfx::Texture get(int x, int y) const {
        return m_texture.slice(x * m_sprite_width, y * m_sprite_height, m_sprite_width, m_sprite_height);
    }

};

} // namespace gfx
