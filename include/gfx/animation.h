#pragma once

#include <cassert>
#include <chrono>
#include <functional>

#include <gfx/interpolators.h>
#include <gfx/lerp.h>

namespace gfx {

using namespace std::chrono_literals;

template <typename T>
concept Animatable = requires (T start, T end, float value) {
{ gfx::lerp(start, end, value) } -> std::same_as<T>;
};

struct IAnimation {
    virtual ~IAnimation() = default;

    virtual void start() = 0;
    virtual void reset() = 0;
    virtual bool is_stopped() const = 0;
    virtual bool is_running() const = 0;
    virtual bool is_done() const = 0;
};

template <Animatable T>
class Animation : public IAnimation {

    using Duration = std::chrono::duration<double>;
    using InterpolationFn = std::function<float(float)>;

    const T m_start;
    const T m_end;
    const Duration m_duration;
    const InterpolationFn m_fn;
    Duration m_start_time = 0s;

    enum class State { Stopped, Running } m_state = State::Stopped;

public:
    Animation(T start, T end, Duration duration, InterpolationFn fn = interpolators::linear)
        : m_start(std::move(start))
        , m_end(std::move(end))
        , m_duration(duration)
        , m_fn(fn)
    { }

    void start() override {
        m_start_time = get_current_time();
        m_state = State::Running;
    }

    void reset() override {
        m_start_time = 0s;
        m_state = State::Stopped;
    }

    [[nodiscard]] bool is_done() const override {
        if (m_state == State::Stopped) return false;
        auto diff = get_current_time() - m_start_time;
        return diff >= m_duration;
    }

    [[nodiscard]] bool is_running() const override {
        return m_state == State::Running;
    }

    [[nodiscard]] bool is_stopped() const override {
        return m_state == State::Stopped;
    }

    [[nodiscard]] T get_start() const {
        return m_start;
    }

    [[nodiscard]] T get_end() const {
        return m_end;
    }

    [[nodiscard]] Duration get_duration() const {
        return m_duration;
    }

    operator T() const {
        return get();
    }

    [[nodiscard]] T get() const {
        switch (m_state) {
            case State::Stopped:
                return m_start;

            case State::Running:
                return is_done()
                ? m_end
                : get(get_current_time() - m_start_time);
        }
    }

    // get the value of the animation independent of its current state
    [[nodiscard]] T get(Duration time) const {
        double t = time / m_duration;
        assert(t <= 1.0f && t >= 0.0f);
        return gfx::lerp(m_start, m_end, m_fn(t));
    }

private:
    [[nodiscard]] static Duration get_current_time() {
        return std::chrono::steady_clock::now().time_since_epoch();
    }

};

} // namespace gfx
