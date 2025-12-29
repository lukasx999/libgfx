#pragma once

#include <gfx.h>

namespace gfx {

class AnimatedTexture {
    enum class State { Idle, Running };

    const gfx::Window& m_window;
    const std::vector<gfx::Texture> m_frames;
    const double m_animation_delay_secs;
    double m_start_time = 0.0;
    bool m_is_looping = true;
    State m_state = State::Idle;

public:
    AnimatedTexture(const gfx::Window& window, std::vector<gfx::Texture> frames, double animation_delay_secs)
        : m_window(window)
        , m_frames(std::move(frames))
        , m_animation_delay_secs(animation_delay_secs)
    { }

    void set_looping(bool is_looping) {
        m_is_looping = is_looping;
    }

    [[nodiscard]] bool is_done() const {
        return m_window.get_time() - m_start_time >= m_frames.size() * m_animation_delay_secs;
    }

    void start() {
        m_state = State::Running;
        m_start_time = m_window.get_time();
    }

    void reset() {
        m_state = State::Idle;
    }

    void draw(gfx::Renderer& rd, gfx::Rect dest) const {
        rd.draw_texture(dest, m_frames[get_current_frame()]);
    }

    // mirrors the sprite along the y-axis
    void draw_mirrored(gfx::Renderer& rd, gfx::Rect dest) const {
        dest.x += dest.width;
        dest.width *= -1;
        draw(rd, dest);
    }

private:
    [[nodiscard]] size_t get_current_frame() const {
        switch (m_state) {
            using enum State;

            case Idle: return 0;

            case Running: {
                double time_passed = m_window.get_time() - m_start_time;
                size_t idx = time_passed / m_animation_delay_secs;
                size_t frame_count = m_frames.size();

                return m_is_looping
                ? idx % frame_count
                : std::min(idx, frame_count-1);

            }
        }
    }

};

} // namespace gfx
