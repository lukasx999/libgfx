#include <print>

#include <gfx/gfx.h>

int main() {
    gfx::Window window(500, 500, "web");

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_circle(window.get_center(), 100, gfx::Color::blue());
    });
}
