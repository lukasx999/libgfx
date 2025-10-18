#include <print>

#include "window.hh"

GLFWwindow* gfx::Window::init_glfw(int width, int height, const char* window_title, uint8_t flags) {

    glfwSetErrorCallback([]([[maybe_unused]] int error, const char* desc) {
        std::println(stderr, "glfw error: {}", desc);
    });

    if (!glfwInit())
        throw std::runtime_error("failed to create window");

    glfwWindowHint(GLFW_RESIZABLE, flags & WindowFlags::Resizable);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("failed to create window");
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(flags & WindowFlags::DisableVsync ? 0 : 1);

    glfwSetInputMode(window, GLFW_CURSOR, flags & WindowFlags::DisableCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO: option for this
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDebugMessageCallback(debug_message_callback, nullptr);

    glfwSetWindowSizeCallback(window, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    return window;
}

int gfx::Window::gfx_mouse_button_to_glfw_mouse_button(MouseButton mb) {
    switch (mb) {
        using enum gfx::MouseButton;

        case LMB: return GLFW_MOUSE_BUTTON_LEFT;
        case RMB: return GLFW_MOUSE_BUTTON_RIGHT;
        case Middle: return GLFW_MOUSE_BUTTON_MIDDLE;
    }
}

int gfx::Window::gfx_key_to_glfw_key(Key key) {
    switch (key) {
        using enum Key;

        case Escape: return GLFW_KEY_ESCAPE;
        case Space: return GLFW_KEY_SPACE;
        case Num0: return GLFW_KEY_0;
        case Num1: return GLFW_KEY_1;
        case Num2: return GLFW_KEY_2;
        case Num3: return GLFW_KEY_3;
        case Num4: return GLFW_KEY_4;
        case Num5: return GLFW_KEY_5;
        case Num6: return GLFW_KEY_6;
        case Num7: return GLFW_KEY_7;
        case Num8: return GLFW_KEY_8;
        case Num9: return GLFW_KEY_9;
        case A: return GLFW_KEY_A;
        case B: return GLFW_KEY_B;
        case C: return GLFW_KEY_C;
        case D: return GLFW_KEY_D;
        case E: return GLFW_KEY_E;
        case F: return GLFW_KEY_F;
        case G: return GLFW_KEY_G;
        case H: return GLFW_KEY_H;
        case I: return GLFW_KEY_I;
        case J: return GLFW_KEY_J;
        case K: return GLFW_KEY_K;
        case L: return GLFW_KEY_L;
        case M: return GLFW_KEY_M;
        case N: return GLFW_KEY_N;
        case O: return GLFW_KEY_O;
        case P: return GLFW_KEY_P;
        case Q: return GLFW_KEY_Q;
        case R: return GLFW_KEY_R;
        case S: return GLFW_KEY_S;
        case T: return GLFW_KEY_T;
        case U: return GLFW_KEY_U;
        case V: return GLFW_KEY_V;
        case W: return GLFW_KEY_W;
        case X: return GLFW_KEY_X;
        case Y: return GLFW_KEY_Y;
        case Z: return GLFW_KEY_Z;
    }
}
