#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <ranges>
#include <functional>

#include <gfx/interpolators.h>
#include <gfx/lerp.h>

namespace gfx {

using namespace std::chrono_literals;

template <typename T>
concept Animatable = requires (T start, T end, float value) {
{ gfx::lerp(start, end, value) } -> std::same_as<T>;
};

// TODO: add duration and state types
// TODO: add get_current_time() helper
struct IAnimation {
    virtual ~IAnimation() = default;

    virtual void start() = 0;
    virtual void reset() = 0;
    [[nodiscard]] virtual bool is_stopped() const = 0;
    [[nodiscard]] virtual bool is_running() const = 0;
    [[nodiscard]] virtual bool is_done() const = 0;
    [[nodiscard]] virtual std::chrono::duration<double> get_duration() const = 0;
};

template <Animatable T>
class Animation : public IAnimation {

    using Duration = std::chrono::duration<double>;
    using InterpolationFn = std::function<float(float)>;

    const T m_start;
    const T m_end;
    const Duration m_duration;
    const InterpolationFn m_fn;

    // 0s means stopped
    Duration m_start_time = 0s;

public:
    Animation(T start, T end, Duration duration, InterpolationFn fn = interpolators::linear)
        : m_start(std::move(start))
        , m_end(std::move(end))
        , m_duration(duration)
        , m_fn(fn)
    { }

    void start() override {
        m_start_time = get_current_time();
    }

    void reset() override {
        m_start_time = 0s;
    }

    [[nodiscard]] bool is_done() const override {
        if (m_start_time == 0s) return false;
        auto diff = get_current_time() - m_start_time;
        return diff >= m_duration;
    }

    [[nodiscard]] bool is_running() const override {
        return m_start_time > 0s;
    }

    [[nodiscard]] bool is_stopped() const override {
        return m_start_time == 0s;
    }

    [[nodiscard]] T get_start() const {
        return m_start;
    }

    [[nodiscard]] T get_end() const {
        return m_end;
    }

    [[nodiscard]] std::chrono::duration<double> get_duration() const override {
        return m_duration;
    }

    operator T() const {
        return get();
    }

    [[nodiscard]] T get() const {
        if (is_stopped()) return m_start;

        return is_done()
        ? m_end
        : get(get_current_time() - m_start_time);
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

class AnimationSequence : public gfx::IAnimation {

    template <typename T>
    using Ref = std::reference_wrapper<T>;

    using Duration = std::chrono::duration<double>;

    const std::vector<Ref<gfx::IAnimation>> m_animations;

    // 0s means stopped
    Duration m_start_time = 0s;

public:
    explicit AnimationSequence(std::initializer_list<Ref<gfx::IAnimation>> animations)
    : m_animations(animations)
    { }

    void dispatch() {
        auto diff = get_current_time() - m_start_time;

        auto current = std::ranges::find_if(m_animations, [&](Ref<gfx::IAnimation> anim) {
            auto duration = anim.get().get_duration();
            if (diff > duration) {
                diff -= duration;
                return false;
            }

            return true;
        });

        if (current == m_animations.end())
            return;

        if (current->get().is_stopped())
            current->get().start();

    }

    void start() override {
        m_start_time = get_current_time();
    }

    void reset() override {
        for (auto& anim : m_animations) {
            anim.get().reset();
        }

        m_start_time = 0s;
    }

    [[nodiscard]] bool is_done() const override {
        if (m_start_time == 0s) return false;
        auto diff = get_current_time() - m_start_time;
        return diff >= get_duration();
    }

    [[nodiscard]] bool is_running() const override {
        return m_start_time > 0s;
    }

    [[nodiscard]] bool is_stopped() const override {
        return m_start_time == 0s;
    }

    [[nodiscard]] std::chrono::duration<double> get_duration() const override {
        return std::ranges::fold_left(m_animations, 0s, [](Duration acc, Ref<gfx::IAnimation> anim) {
            return acc + anim.get().get_duration();
        });
    }


private:
    [[nodiscard]] static Duration get_current_time() {
        return std::chrono::steady_clock::now().time_since_epoch();
    }

};

} // namespace gfx
