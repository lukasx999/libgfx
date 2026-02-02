#pragma once

#include <cassert>
#include <chrono>
#include <functional>

#include <gfx/color.h>
#include <gfx/rect.h>
#include <gfx/interpolators.h>

namespace gfx {

using namespace std::chrono_literals;

// TODO: upgrade to C++26 delete with reason syntax at some point

template <typename T>
T lerp(T, T, float) {
    static_assert(false, R"(
    no matching gfx::lerp<T>() specialization was found.
        to fix this:
        - either create a template specialization for your type,
        - or implement `operator+(T)`, `operator-(T)` and `operator*(float)` on your type.)");
}

// we use x as the right hand operand, because when overloading operator*(float) in a class,
// the float operand is on the right hand side. to have it as the left hand operator, you would
// need to define the operator outside of the class, which some users might forget about.

template <typename T>
concept TriviallyLerpable = requires (T start, T end, float x) {
start + (end - start) * x;
};

template <TriviallyLerpable T>
[[nodiscard]] inline constexpr T lerp(T start, T end, float x) {
    return start + (end - start) * x;
}

template <>
[[nodiscard]] inline constexpr
gfx::Color lerp<gfx::Color>(gfx::Color start, gfx::Color end, float x) {
    return {
        gfx::lerp(start.r, end.r, x),
        gfx::lerp(start.g, end.g, x),
        gfx::lerp(start.b, end.b, x),
        gfx::lerp(start.a, end.a, x),
    };
}

template <>
[[nodiscard]] inline constexpr
gfx::Rect lerp<gfx::Rect>(gfx::Rect start, gfx::Rect end, float x) {
    return {
        gfx::lerp(start.x, end.x, x),
        gfx::lerp(start.y, end.y, x),
        gfx::lerp(start.width, end.width, x),
        gfx::lerp(start.height, end.height, x),
    };
}

template <typename T>
concept Animatable = requires (T start, T end, float value) {
{ gfx::lerp(start, end, value) } -> std::same_as<T>;
};

template <Animatable T>
class Animation {

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
        : m_start(start)
        , m_end(end)
        , m_duration(duration)
        , m_fn(fn)
    { }

    void start() {
        m_start_time = get_current_time();
        m_state = State::Running;
    }

    void reset() {
        m_start_time = 0s;
        m_state = State::Stopped;
    }

    [[nodiscard]] bool is_done() const {
        if (m_state == State::Stopped) return false;
        auto diff = get_current_time() - m_start_time;
        return diff >= m_duration;
    }

    [[nodiscard]] bool is_running() const {
        return m_state == State::Running;
    }

    [[nodiscard]] bool is_stopped() const {
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
                return is_done() ? m_end : get_running();
        }
    }

private:
    [[nodiscard]] T get_running() const {
        auto diff = get_current_time() - m_start_time;
        double t = diff / m_duration;
        assert(t <= 1.0f);
        return gfx::lerp(m_start, m_end, m_fn(t));
    }

    [[nodiscard]] static Duration get_current_time() {
        return std::chrono::steady_clock::now().time_since_epoch();
    }

};

} // namespace gfx
