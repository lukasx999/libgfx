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

class AnimationBase {
public:
    using Duration = std::chrono::duration<double>;

    virtual ~AnimationBase() = default;

    [[nodiscard]] virtual Duration get_duration() const = 0;

    // TODO: hide deadtime from the user api?
    void set_dead_time(Duration duration) {
        m_dead_time = duration;
    }

    [[nodiscard]] Duration get_dead_time() const {
        return m_dead_time;
    }

    void restart() {
        reset();
        start();
    }

    virtual void start() {
        m_start_time = get_current_time();
    }

    virtual void reset() {
        m_start_time = 0s;
    }

    [[nodiscard]] virtual bool is_stopped() const {
        return m_start_time == 0s;
    }

    [[nodiscard]] virtual bool is_running() const {
        return m_start_time > 0s;
    }

    [[nodiscard]] virtual bool is_done() const {
        if (m_start_time == 0s) return false;
        auto diff = get_current_time() - m_start_time;
        return diff >= get_duration();
    }

protected:
    // time the animation waits until it starts playing. used for running sequential animations
    Duration m_dead_time = 0s;

    // 0s means stopped
    Duration m_start_time = 0s;

    [[nodiscard]] static Duration get_current_time() {
        return std::chrono::steady_clock::now().time_since_epoch();
    }

};

template <Animatable T>
class Animation : public AnimationBase {
public:
    using InterpolationFn = std::function<float(float)>;

    Animation(T start, T end, Duration duration, InterpolationFn fn = interpolators::linear)
        : m_start(std::move(start))
        , m_end(std::move(end))
        , m_duration(duration)
        , m_fn(fn)
    { }

    [[nodiscard]] T get_start() const {
        return m_start;
    }

    [[nodiscard]] T get_end() const {
        return m_end;
    }

    [[nodiscard]] Duration get_duration() const override {
        return m_duration + m_dead_time;
    }

    operator T() const {
        return get();
    }

    [[nodiscard]] T get() const {
        if (is_stopped()) return m_start;
        if (is_done()) return m_end;

        auto diff = get_current_time() - (m_start_time + m_dead_time);

        // inside of deadtime
        if (diff < 0s) return m_start;

        assert(is_running());
        return get(diff);
    }

    [[nodiscard]] T get(Duration diff) const {
        double t = diff / m_duration;
        assert(t <= 1.0f && t >= 0.0f);
        return gfx::lerp(m_start, m_end, m_fn(t));
    }

private:
    const T m_start;
    const T m_end;
    const Duration m_duration;
    const InterpolationFn m_fn;

};

class AnimationSequence : public gfx::AnimationBase {

    template <typename T>
    using Ref = std::reference_wrapper<T>;

public:
    explicit AnimationSequence(std::initializer_list<Ref<gfx::AnimationBase>> animations)
    : m_animations(animations)
    { }

    void start() override {
        AnimationBase::start();

        Duration dead_time = m_dead_time;
        for (auto& animation : m_animations) {
            auto& anim = animation.get();
            anim.set_dead_time(dead_time);
            anim.start();
            dead_time += anim.get_duration();
        }
    }

    void reset() override {
        AnimationBase::reset();
        for (auto& anim : m_animations) {
            anim.get().reset();
        }
    }

    [[nodiscard]] Duration get_duration() const override {
        return m_animations.back().get().get_duration();
    }

private:
    const std::vector<Ref<gfx::AnimationBase>> m_animations;

};

class AnimationBatch : public gfx::AnimationBase {

    template <typename T>
    using Ref = std::reference_wrapper<T>;

public:
    explicit AnimationBatch(std::initializer_list<Ref<gfx::AnimationBase>> animations)
    : m_animations(animations)
    { }

    void start() override {
        AnimationBase::start();
        for (auto& anim : m_animations) {
            anim.get().set_dead_time(m_dead_time);
            anim.get().start();
        }
    }

    void reset() override {
        AnimationBase::reset();
        for (auto& anim : m_animations) {
            anim.get().reset();
        }
    }

    [[nodiscard]] Duration get_duration() const override {
        auto max = std::ranges::max_element(m_animations, [](Ref<gfx::AnimationBase> a, decltype(a) b) {
            return a.get().get_duration() < b.get().get_duration();
        });
        assert(max != m_animations.end());

        return max->get().get_duration();
    }

private:
    const std::vector<Ref<gfx::AnimationBase>> m_animations;

};

} // namespace gfx
