#pragma once

#include <unordered_map>
#include <span>
#include <vector>

#include <gfx.h>

template <typename Key>
class Spritesheet {
    const gfx::Texture& m_texture;
    std::unordered_map<Key, std::vector<gfx::Rect>> m_sprite_map;
    mutable size_t m_anim_idx = 0;
    mutable double m_anim_start = 0.0f;
    const double m_anim_delay_secs;

public:
    Spritesheet(const gfx::Texture& texture, double anim_delay_secs)
        : m_texture(texture)
        , m_anim_delay_secs(anim_delay_secs)
    { }

    Spritesheet(const gfx::Texture& texture, double anim_delay_secs, std::span<const std::pair<Key, std::vector<gfx::Rect>>> initial_sprites)
    : Spritesheet(texture, anim_delay_secs)
    {
        for (auto& [key, rects] : initial_sprites) {
            set_sprite(key, rects);
        }
    }

    [[nodiscard]] const gfx::Texture& get_texture() const {
        return m_texture;
    }

    void set_sprite(Key key, std::vector<gfx::Rect> src) {
        m_sprite_map[key] = std::move(src);
    }

    // TODO: somehow remove mutable from the start and idx
    void draw(gfx::Renderer& rd, Key key, gfx::Rect dest) const {
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
    void draw_mirrored(gfx::Renderer& rd, Key key, gfx::Rect dest) const {
        dest.x += dest.width;
        dest.width *= -1;
        draw(rd, key, dest);
    }

};
