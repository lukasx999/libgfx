#include <emscripten.h>
#include <gfx/gfx.h>

void loop(void* arg) {
    gfx::Window& window = *static_cast<gfx::Window*>(arg);

    window.with_draw_loop_context([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::blue());
        rd.draw_circle(window.get_center(), 100, gfx::Color::red());
    });

}

int main() {
    gfx::Window window(500, 500, "example");
    emscripten_set_main_loop_arg(loop, &window, 0, true);
}
