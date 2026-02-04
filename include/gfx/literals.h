#pragma once

#include <gfx/rotation.h>

namespace gfx::literals {

inline consteval gfx::Degrees operator""_deg(unsigned long long value) {
    return gfx::Degrees(value);
}

inline consteval gfx::Radians operator""_rad(unsigned long long value) {
    return gfx::Radians(value);
}

} // namespace gfx::literals

#ifndef LIBGFX_DISABLE_LITERALS
using namespace gfx::literals;
#endif // LIBGFX_DISABLE_LITERALS
