#include <gfx/gfx.h>

int main() {

    gfx::Window window(700, 700, "quadratic bezier curve");

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        gfx::Vec a(100, 500);
        gfx::Vec b(600, 500);
        auto ctl = window.get_mouse_pos();

        rd.draw_circle(a, 5, gfx::Color::white());
        rd.draw_circle(b, 5, gfx::Color::white());
        rd.draw_circle(ctl, 5, gfx::Color::red());

        rd.draw_quadratic_bezier_curve(a, b, ctl, gfx::Color::white());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
