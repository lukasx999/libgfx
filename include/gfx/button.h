#pragma once

#include <gfx/rect.h>
#include <gfx/window.h>
#include <gfx/font.h>
#include <gfx/renderer.h>
#include <gfx/color.h>

namespace gfx::ui {

// TODO: make sure fontsize doesnt exceed bounding box m_rect
class Button {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

    const char* m_label;
    const gfx::Rect m_rect;
    const int m_fontsize = 50;

    const gfx::Color m_color_idle = gfx::Color::grey();
    const gfx::Color m_color_hovered = gfx::Color::white();
    const gfx::Color m_color_pressed = gfx::Color::blue();

    enum class State {
        Idle, Hovered, Pressed,
    } m_state = State::Idle;

    using Callback = std::function<void()>;

    Callback m_on_idle  = [] {};
    Callback m_on_hover = [] {};
    Callback m_on_press = [] {};

public:
    Button(const gfx::Window& window, const gfx::Font& font, const char* label, gfx::Rect rect, int fontsize)
        : m_window(window)
        , m_font(font)
        , m_label(label)
        , m_rect(rect)
        , m_fontsize(fontsize)
    { }

    [[nodiscard]] State get_state() const {
        return m_state;
    }

    [[nodiscard]] bool is_idle() const {
        return m_state == State::Idle;
    }

    [[nodiscard]] bool is_hovered() const {
        return m_state == State::Hovered;
    }

    [[nodiscard]] bool is_pressed() const {
        return m_state == State::Pressed;
    }

    [[nodiscard]] const char* get_label() const {
        return m_label;
    }

    void set_label(const char* label) {
        m_label = label;
    }

    void draw(gfx::Renderer& rd) const {

        auto color = [&] {
            switch (m_state) {
                using enum State;
                case Idle: return m_color_idle;
                case Hovered: return m_color_hovered;
                case Pressed: return m_color_pressed;
            }
        }();

        rd.draw_rectangle(m_rect, color);
        rd.draw_text_centered(
            m_rect.x + m_rect.width/2.0,
            m_rect.y + m_rect.height/2.0 - m_fontsize/2.0,
            m_fontsize,
            m_label,
            m_font,
            gfx::Color::red()
        );
    }

    void update() {
        bool is_hovered = m_rect.check_collision_point(m_window.get_mouse_pos());
        bool is_pressed = m_window.get_mouse_button_state(gfx::MouseButton::Left).pressed();

        auto new_state = is_hovered && is_pressed ?
            State::Pressed
            : is_hovered ? State::Hovered
            : State::Idle;

        if (m_state != new_state)
            on_state_change(new_state);

        m_state = new_state;

    }

    void on_idle(Callback callback) {
        m_on_idle = callback;
    }

    void on_hover(Callback callback) {
        m_on_hover = callback;
    }

    void on_press(Callback callback) {
        m_on_press = callback;
    }

private:
    void on_state_change(State new_state) {
        switch (new_state) {
            using enum State;
            case Idle: return m_on_idle();
            case Hovered: return m_on_hover();
            case Pressed: return m_on_press();
        }
    }

};

} // namespace gfx::ui
