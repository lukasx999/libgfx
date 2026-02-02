#pragma once

#include <vector>
#include <chrono>

#include <gfx/window.h>
#include <gfx/texture.h>
#include <gfx/rect.h>

namespace gfx {

using namespace std::chrono_literals;

class AnimatedTexture {

    using Duration = std::chrono::duration<double>;

    const std::vector<gfx::Texture> m_frames;
    const Duration m_delay;
    Duration m_start_time = 0s;
    bool m_is_looping = true;
    enum class State { Stopped, Running } m_state = State::Stopped;

public:
    AnimatedTexture(std::vector<gfx::Texture> frames, Duration delay)
        : m_frames(std::move(frames))
        , m_delay(delay)
    { }

    void set_looping(bool is_looping) {
        m_is_looping = is_looping;
    }

    [[nodiscard]] bool is_done() const {
        return get_current_time() - m_start_time >= m_frames.size() * m_delay;
    }

    void start() {
        m_state = State::Running;
        m_start_time = get_current_time();
    }

    void reset() {
        m_state = State::Stopped;
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

            case Stopped: return 0;

            case Running: {
                auto time_passed = get_current_time() - m_start_time;
                size_t idx = time_passed / m_delay;
                size_t frame_count = m_frames.size();

                return m_is_looping
                ? idx % frame_count
                : std::min(idx, frame_count - 1);

            }
        }
    }

    [[nodiscard]] static Duration get_current_time() {
        return std::chrono::steady_clock::now().time_since_epoch();
    }

};

} // namespace gfx
