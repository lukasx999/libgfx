#pragma once

#include <cmath>

namespace gfx::interpolators {

[[nodiscard]] inline constexpr float step([[maybe_unused]] float x) noexcept {
    return 1.0f;
}

[[nodiscard]] inline constexpr float linear(float x) noexcept {
    return x;
}

[[nodiscard]] inline constexpr float ease_in_quad(float x) noexcept {
    return std::pow(x, 2);
}

[[nodiscard]] inline constexpr float ease_in_out_quad(float x) noexcept {
    return x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2;
}

[[nodiscard]] inline constexpr float ease_in_cubic(float x) noexcept {
    return std::pow(x, 3);
}

[[nodiscard]] inline constexpr float ease_out_expo(float x) noexcept {
    return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
}

[[nodiscard]] inline constexpr float ease_in_out_cubic(float x) noexcept {
    return x < 0.5 ? 4 * std::pow(x, 3) : 1 - std::pow(-2 * x + 2, 3) / 2;
}

[[nodiscard]] inline constexpr float ease_in_out_back(float x) noexcept {
    float c1 = 1.70158;
    float c2 = c1 * 1.525;

    return x < 0.5
    ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
    : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

[[nodiscard]] inline constexpr float ease_in_out_circ(float x) noexcept {
    return x < 0.5
    ? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
    : (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}

[[nodiscard]] inline constexpr float ease_in_out_quint(float x) noexcept {
    return x < 0.5 ? 16 * std::pow(x, 5) : 1 - std::pow(-2 * x + 2, 5) / 2;
}

[[nodiscard]] inline constexpr float ease_out_elastic(float x) noexcept {
    float c4 = (2 * M_PI) / 3;
    return x == 0 ? 0 : x == 1 ? 1 : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
}

[[nodiscard]] inline constexpr float ease_in_expo(float x) noexcept {
    return x == 0 ? 0 : std::pow(2, 10 * x - 10);
}

[[nodiscard]] inline constexpr float ease_out_back(float x) noexcept {
    float c1 = 1.70158;
    float c3 = c1 + 1;
    return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
}

[[nodiscard]] inline constexpr float ease_in_out_expo(float x) noexcept {
    return x == 0 ? 0 : x == 1 ? 1
    : x < 0.5 ? std::pow(2, 20 * x - 10) / 2
    : (2 - std::pow(2, -20 * x + 10)) / 2;
}

} // namespace gfx::interpolators
