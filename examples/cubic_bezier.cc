#include <print>

#include <gfx/gfx.h>

int main() {

    gfx::Window window(700, 700, "quadratic bezier curve");

    float handle_radius = 10;

    bool moving_a = false;
    bool moving_b = false;
    bool moving_ctl1 = false;
    bool moving_ctl2 = false;

    gfx::Vec a(100, 500);
    gfx::Vec b(600, 500);
    gfx::Vec ctl1(250, 100);
    gfx::Vec ctl2(450, 100);

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        auto mouse = window.get_mouse_pos();

        if (window.get_mouse_button_state(gfx::MouseButton::Left).pressed()) {
            if (mouse.distance(a) <= handle_radius)
                moving_a = true;

            if (mouse.distance(b) <= handle_radius)
                moving_b = true;

            if (mouse.distance(ctl1) <= handle_radius)
                moving_ctl1 = true;

            if (mouse.distance(ctl2) <= handle_radius)
                moving_ctl2 = true;

        }

        if (moving_a)
            a = mouse;

        if (moving_b)
            b = mouse;

        if (moving_ctl1)
            ctl1 = mouse;

        if (moving_ctl2)
            ctl2 = mouse;

        if (window.get_mouse_button_state(gfx::MouseButton::Left).released()) {
            moving_a = false;
            moving_b = false;
            moving_ctl1 = false;
            moving_ctl2 = false;
        }

        rd.draw_line_thick(a, ctl1, 2, gfx::Color::gray());
        rd.draw_line_thick(b, ctl2, 2, gfx::Color::gray());

        rd.draw_cubic_bezier_curve(a, b, ctl1, ctl2, 5, gfx::Color::white());

        rd.draw_circle(a, handle_radius, gfx::Color::blue());
        rd.draw_circle(b, handle_radius, gfx::Color::blue());
        rd.draw_circle(ctl1, handle_radius, gfx::Color::red());
        rd.draw_circle(ctl2, handle_radius, gfx::Color::red());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
