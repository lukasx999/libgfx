#pragma once

#include <cassert>
#include <chrono>
#include <functional>
#include <cmath>

#include <gfx/color.h>
#include <gfx/vec.h>

namespace gfx {

namespace interpolators {

[[nodiscard]] inline constexpr float step([[maybe_unused]] float x) {
    return 1.0f;
}

[[nodiscard]] inline constexpr float linear(float x) {
    return x;
}

[[nodiscard]] inline constexpr float ease_in_quad(float x) {
    return std::pow(x, 2);
}

[[nodiscard]] inline constexpr float ease_in_out_quad(float x) {
    return x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2;
}

[[nodiscard]] inline constexpr float ease_in_cubic(float x) {
    return std::pow(x, 3);
}

[[nodiscard]] inline constexpr float ease_out_expo(float x) {
    return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
}

[[nodiscard]] inline constexpr float ease_in_out_cubic(float x) {
    return x < 0.5 ? 4 * std::pow(x, 3) : 1 - std::pow(-2 * x + 2, 3) / 2;
}

[[nodiscard]] inline constexpr float ease_in_out_back(float x) {
    float c1 = 1.70158;
    float c2 = c1 * 1.525;

    return x < 0.5
    ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
    : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

[[nodiscard]] inline constexpr float ease_in_out_circ(float x) {
    return x < 0.5
    ? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
    : (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}

[[nodiscard]] inline constexpr float ease_in_out_quint(float x) {
    return x < 0.5 ? 16 * std::pow(x, 5) : 1 - std::pow(-2 * x + 2, 5) / 2;
}

[[nodiscard]] inline constexpr float ease_out_elastic(float x) {
    float c4 = (2 * M_PI) / 3;
    return x == 0 ? 0 : x == 1 ? 1 : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
}

[[nodiscard]] inline constexpr float ease_in_expo(float x) {
    return x == 0 ? 0 : std::pow(2, 10 * x - 10);
}

[[nodiscard]] inline constexpr float ease_out_back(float x) {
    float c1 = 1.70158;
    float c3 = c1 + 1;
    return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
}

[[nodiscard]] inline constexpr float ease_in_out_expo(float x) {
    return x == 0 ? 0 : x == 1 ? 1
    : x < 0.5 ? std::pow(2, 20 * x - 10) / 2
    : (2 - std::pow(2, -20 * x + 10)) / 2;
}

} // namespace interpolators

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

    enum class State {
        Idle,
        Running,
    } m_state = State::Idle;

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
        m_state = State::Idle;
    }

    [[nodiscard]] bool is_done() const {
        if (m_state == State::Idle) return false;
        auto diff = get_current_time() - m_start_time;
        return diff >= m_duration;
    }

    [[nodiscard]] bool is_running() const {
        return m_state == State::Running;
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
            case State::Idle:
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
