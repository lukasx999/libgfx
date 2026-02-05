#pragma once

#include <vector>

#include <gfx/texture.h>
#include <gfx/rotation.h>
#include <gfx/renderer.h>

namespace gfx {

class Spritesheet {
    const gfx::Texture& m_texture;
    const float m_sprite_width;
    const float m_sprite_height;

public:
    Spritesheet(const gfx::Texture& texture, float cell_width, float cell_height)
        : m_texture(texture)
        , m_sprite_width(cell_width)
        , m_sprite_height(cell_height)
    { }

    [[nodiscard]] float get_cell_width() const {
        return m_sprite_width;
    }

    [[nodiscard]] float get_cell_height() const {
        return m_sprite_height;
    }

    void draw(gfx::Renderer& rd, gfx::Rect dest, gfx::Vec src, gfx::Rotation rotation=0_deg) const {
        rd.draw_texture_sub(dest, { src.x, src.y, m_sprite_width, m_sprite_height }, m_texture, rotation);
    }

    void draw(gfx::Renderer& rd, gfx::Rect dest, float src_x, float src_y, auto&&... args) const {
        draw(rd, dest, { src_x, src_y }, std::forward<decltype(args)>(args)...);
    }

    [[nodiscard]] std::vector<gfx::Texture> get_row(int y, int n) const {
        std::vector<gfx::Texture> row;

        for (int x = 0; x < n; ++x)
            row.push_back(get(x, y));

        return row;
    }

    [[nodiscard]] gfx::Texture get(float x, float y) const {
        return m_texture.slice(x * m_sprite_width, y * m_sprite_height, m_sprite_width, m_sprite_height);
    }

};

} // namespace gfx
