#include <gfx/gfx.h>

int main() {

    gfx::Window window(1600, 900, "shapes");

    window.draw_loop([&](gfx::Renderer& rd) {

        rd.clear_background(gfx::Color::black());

        float spacing = 50;
        rd.draw_rectangle(
            spacing,
            spacing,
            window.get_width()  - spacing * 2,
            window.get_height() - spacing * 2,
            gfx::Color::gray()
        );

        rd.draw_triangle(
            spacing, window.get_height() - spacing,
            window.get_width() - spacing, window.get_height() - spacing,
            window.get_width() / 2.0, spacing,
            gfx::Color::blue()
        );

        rd.draw_circle(window.get_midpoint(), 100, gfx::Color::white());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    });

}
