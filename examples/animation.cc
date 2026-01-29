#include <gfx/gfx.h>


int main() {

    using namespace std::chrono_literals;

    gfx::Window window(1600, 900, "animation");

    gfx::Animation<float> radius(50.0f, 300.0f, 3s, gfx::interpolators::ease_in_quad);
    radius.start();

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_circle(window.get_center(), radius, gfx::Color::white());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    });

}
