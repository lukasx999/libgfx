#include <print>

#include <gfx/gfx.h>

int main() {

    gfx::Window window(700, 700, "quadratic bezier curve");

    float handle_radius = 10;
    float handle_radius_moving = 15;

    bool moving_a = false;
    bool moving_b = false;
    bool moving_ctl = false;

    gfx::Vec a(100, 500);
    gfx::Vec b(600, 500);
    gfx::Vec ctl(350, 100);

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        auto mouse = window.get_mouse_pos();

        bool hovering_a = mouse.distance(a) <= handle_radius;
        bool hovering_b = mouse.distance(b) <= handle_radius;
        bool hovering_ctl = mouse.distance(ctl) <= handle_radius;

        if (window.get_mouse_button_state(gfx::MouseButton::Left).pressed()) {
            if (hovering_a)
                moving_a = true;

            if (hovering_b)
                moving_b = true;

            if (hovering_ctl)
                moving_ctl = true;
        }

        if (window.get_mouse_button_state(gfx::MouseButton::Left).released()) {
            moving_a = false;
            moving_b = false;
            moving_ctl = false;
        }

        if (moving_a)
            a = mouse;

        if (moving_b)
            b = mouse;

        if (moving_ctl)
            ctl = mouse;

        rd.draw_quadratic_bezier_curve(a, b, ctl, 5, gfx::Color::white());

        rd.draw_circle(a, hovering_a ? handle_radius_moving : handle_radius, gfx::Color::blue());
        rd.draw_circle(b, hovering_b ? handle_radius_moving : handle_radius, gfx::Color::blue());
        rd.draw_circle(ctl, hovering_ctl ? handle_radius_moving : handle_radius, gfx::Color::red());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
