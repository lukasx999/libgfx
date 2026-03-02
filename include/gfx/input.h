#pragma once

namespace gfx {

class KeyState {
public:
    // returns true if the key is being pressed right now
    [[nodiscard]] bool is_pressed() const;
    // returns the rising edge of the key, meaning if its being held down, it
    // will only be true once
    [[nodiscard]] bool is_clicked() const;
    [[nodiscard]] bool is_released() const;
    [[nodiscard]] bool is_repeated() const;

private:
    friend class Window;
    int m_glfw_state;
    explicit KeyState(int glfw_state) : m_glfw_state(glfw_state) { }
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
