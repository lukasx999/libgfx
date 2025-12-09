#pragma once

#include <unordered_map>

#include <gfx.h>

template <typename Key>
class Spritesheet {
    const gfx::Texture& m_texture;
    std::unordered_map<Key, gfx::Rect> m_sprite_map;

public:
    explicit Spritesheet(const gfx::Texture& texture) : m_texture(texture) { }

    void set_sprite(Key key, gfx::Rect src) {
        m_sprite_map[key] = src;
    }

    void draw(gfx::Renderer& rd, Key key, gfx::Rect dest) const {
        rd.draw_texture_sub(dest, m_sprite_map.at(key), m_texture);
    }

    // mirrors the sprite along the y-axis
    void draw_mirrored(gfx::Renderer& rd, Key key, gfx::Rect dest) const {
        dest.x += dest.width;
        dest.width *= -1;
        rd.draw_texture_sub(dest, m_sprite_map.at(key), m_texture);
    }

};
