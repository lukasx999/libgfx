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
protected:
    using Duration = std::chrono::duration<double>;

    // 0s means stopped
    Duration m_start_time = 0s;

public:
    virtual ~AnimationBase() = default;

    [[nodiscard]] virtual std::chrono::duration<double> get_duration() const = 0;

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
    [[nodiscard]] static Duration get_current_time() {
        return std::chrono::steady_clock::now().time_since_epoch();
    }
};

template <Animatable T>
class Animation : public AnimationBase {

    using InterpolationFn = std::function<float(float)>;

    const T m_start;
    const T m_end;
    const Duration m_duration;
    const InterpolationFn m_fn;

public:
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

};

class AnimationSequence : public gfx::AnimationBase {

    template <typename T>
    using Ref = std::reference_wrapper<T>;

    const std::vector<Ref<gfx::AnimationBase>> m_animations;

public:
    explicit AnimationSequence(std::initializer_list<Ref<gfx::AnimationBase>> animations)
    : m_animations(animations)
    { }

    void dispatch() {

        for (auto& anim : m_animations) {
            auto ptr = dynamic_cast<AnimationSequence*>(&anim.get());
            if (ptr != nullptr)
                ptr->dispatch();
        }

        auto diff = get_current_time() - m_start_time;

        auto current = std::ranges::find_if(m_animations, [&](Ref<gfx::AnimationBase> anim) {
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

    void reset() override {
        AnimationBase::reset();
        for (auto& anim : m_animations) {
            anim.get().reset();
        }
    }

    [[nodiscard]] std::chrono::duration<double> get_duration() const override {
        return std::ranges::fold_left(m_animations, 0s, [](Duration acc, Ref<gfx::AnimationBase> anim) {
            return acc + anim.get().get_duration();
        });
    }

};

class AnimationBatch : public gfx::AnimationBase {

    template <typename T>
    using Ref = std::reference_wrapper<T>;

    const std::vector<Ref<gfx::AnimationBase>> m_animations;

public:
    explicit AnimationBatch(std::initializer_list<Ref<gfx::AnimationBase>> animations)
    : m_animations(animations)
    { }

    void start() override {
        AnimationBase::start();
        for (auto& anim : m_animations) {
            anim.get().start();
        }
    }

    void reset() override {
        AnimationBase::reset();
        for (auto& anim : m_animations) {
            anim.get().reset();
        }
    }

    [[nodiscard]] std::chrono::duration<double> get_duration() const override {
        auto max = std::ranges::max_element(m_animations, [](Ref<gfx::AnimationBase> a, decltype(a) b) {
            return a.get().get_duration() < b.get().get_duration();
        });
        assert(max != m_animations.end());

        return max->get().get_duration();
    }

};

} // namespace gfx
