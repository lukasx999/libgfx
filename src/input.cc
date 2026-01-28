#include <gfx/input.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace gfx {

bool KeyState::pressed() const {
    return m_glfw_state == GLFW_PRESS;
}

bool KeyState::released() const {
    return m_glfw_state == GLFW_RELEASE;
}

bool KeyState::repeated() const {
    return m_glfw_state == GLFW_REPEAT;
}

} // namespace gfx
