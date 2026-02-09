#include <print>

#include <emscripten.h>
#include <gfx/gfx.h>

void loop(void* arg) {
    gfx::Window& window = *static_cast<gfx::Window*>(arg);

    window.with_draw_loop_context([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_circle(window.get_center(), 100, gfx::Color::blue());
    });

}

int main() {
    gfx::Window window(500, 500, "foo");
    emscripten_set_main_loop_arg(loop, &window, 0, true);
}
