#include <gfx/gfx.h>

int main() {

    gfx::Window window(700, 700, "hello world");

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_rectangle(0, 0, 100, 100, gfx::Color::white());
    });

}
