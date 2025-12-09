#pragma once

#include <unordered_map>

#include <gfx.h>

template <typename Key>
class Spritesheet {
    const gfx::Texture& m_texture;
    std::unordered_map<Key, std::vector<gfx::Rect>> m_sprite_map;
    size_t m_anim_idx = 0;
    double m_anim_start = 0.0f;
    const double m_anim_delay_secs;

public:
    Spritesheet(const gfx::Texture& texture, double anim_delay_secs)
        : m_texture(texture)
        , m_anim_delay_secs(anim_delay_secs)
    { }

    [[nodiscard]] const gfx::Texture& get_texture() const {
        return m_texture;
    }

    void set_sprite(Key key, std::vector<gfx::Rect> src) {
        m_sprite_map[key] = std::move(src);
    }

    void draw(gfx::Renderer& rd, Key key, gfx::Rect dest) {
        auto textures = m_sprite_map.at(key);
        auto texture = textures[m_anim_idx];

        double diff = rd.get_window().get_time() - m_anim_start;
        if (diff >= m_anim_delay_secs) {
            m_anim_idx++;

            if (m_anim_idx >= textures.size())
                m_anim_idx = 0;

            m_anim_start = rd.get_window().get_time();
        }

        rd.draw_texture_sub(dest, texture, m_texture);
    }

    // mirrors the sprite along the y-axis
    void draw_mirrored(gfx::Renderer& rd, Key key, gfx::Rect dest) {
        dest.x += dest.width;
        dest.width *= -1;
        draw(rd, key, dest);
    }

};
