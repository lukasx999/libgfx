#include <gfx/input.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace gfx {

bool KeyState::is_pressed() const {
    return m_glfw_state == GLFW_PRESS;
}

bool KeyState::is_clicked() const {
    static bool old_press = false;
    bool press = is_pressed();
    bool click = press && !old_press;
    old_press = press;
    return click;
}

bool KeyState::is_released() const {
    return m_glfw_state == GLFW_RELEASE;
}

bool KeyState::is_repeated() const {
    return m_glfw_state == GLFW_REPEAT;
}

} // namespace gfx
