#pragma once

#include <gfx/color.h>
#include <gfx/rect.h>

namespace gfx {

// TODO: upgrade to C++26 delete with reason syntax at some point

template <typename T>
T lerp(T, T, float) {
    static_assert(false, R"(
    no matching gfx::lerp<T>() specialization was found.
        to fix this:
        - either create a template specialization for your type,
        - or implement `operator+(T)`, `operator-(T)` and `operator*(float)` on your type.)");
}

// if a type can be trivially interpolated by just using operator+(),
// operator-() and operator()*, then implement it in such way, else look for another
// template specialization.
//
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

} // namespace gfx
