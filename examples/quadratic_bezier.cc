#include <print>

#include <gfx/gfx.h>

int main() {

    gfx::Window window(700, 700, "quadratic bezier curve");

    float handle_radius = 10;

    bool moving_a = false;
    bool moving_b = false;
    bool moving_ctl = false;

    gfx::Vec a(100, 500);
    gfx::Vec b(600, 500);
    gfx::Vec ctl(350, 100);

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        auto mouse = window.get_mouse_pos();

        if (window.get_mouse_button_state(gfx::MouseButton::Left).pressed()) {
            if (mouse.distance(a) <= handle_radius)
                moving_a = true;

            if (mouse.distance(b) <= handle_radius)
                moving_b = true;

            if (mouse.distance(ctl) <= handle_radius)
                moving_ctl = true;

        }

        if (moving_a)
            a = mouse;

        if (moving_b)
            b = mouse;

        if (moving_ctl)
            ctl = mouse;

        if (window.get_mouse_button_state(gfx::MouseButton::Left).released()) {
            moving_a = false;
            moving_b = false;
            moving_ctl = false;
        }



        rd.draw_quadratic_bezier_curve(a, b, ctl, 5, gfx::Color::white());

        rd.draw_circle(a, handle_radius, gfx::Color::blue());
        rd.draw_circle(b, handle_radius, gfx::Color::blue());
        rd.draw_circle(ctl, handle_radius, gfx::Color::red());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
