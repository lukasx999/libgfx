#include <gfx.h>

int main() {

    gfx::Window window(500, 500, "shapes");

    window.draw_loop([&](gfx::Renderer& rd) {

        rd.clear_background(gfx::Color::black());

        rd.draw_rectangle({ 50, 50, 400, 400 }, gfx::Color::blue());
        rd.draw_triangle({ 250, 50 }, { 50, 450 }, { 450, 450 }, gfx::Color::orange());
        rd.draw_circle(window.get_midpoint(), 75, gfx::Color::white());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    });

}
