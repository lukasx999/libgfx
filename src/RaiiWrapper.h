#pragma once

#include <concepts>

template <typename T>
using DefaultDeleter = decltype([](T) { });

template <typename T, std::invocable<T> Deleter = DefaultDeleter<T>>
class RaiiWrapper {
    T m_value;

public:
    constexpr RaiiWrapper(T value) : m_value(value) { }

    constexpr RaiiWrapper(const RaiiWrapper&) = default;
    constexpr RaiiWrapper& operator=(const RaiiWrapper&) = default;
    constexpr RaiiWrapper& operator=(RaiiWrapper&&) = default;
    constexpr RaiiWrapper(RaiiWrapper&&) = default;

    constexpr ~RaiiWrapper() {
        Deleter deleter;
        deleter(m_value);
    }

    constexpr operator T() const {
        return m_value;
    }

};
