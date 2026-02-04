#pragma once

#include <string>
#include <stdexcept>

namespace gfx {

struct Error : std::runtime_error {
    explicit Error(const char* msg) : std::runtime_error(msg) { }
    explicit Error(const std::string& msg) : std::runtime_error(msg) { }
};

} // namespace gfx
