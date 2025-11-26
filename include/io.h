#pragma once

namespace gfx {

class KeyState {
    friend class Window;
    int m_glfw_state;
    explicit KeyState(int glfw_state) : m_glfw_state(glfw_state) { }

public:
    [[nodiscard]] bool pressed() const;
    [[nodiscard]] bool released() const;
    [[nodiscard]] bool repeated() const;

};

enum class MouseButton {
    Left, Right, Middle,
};

enum class Key {
    Escape,
    Space,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
};

} // namespace gfx
