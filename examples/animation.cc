#include <gfx/gfx.h>

int main() {

    using namespace std::chrono_literals;

    gfx::Window window(700, 700, "animation");

    gfx::Animation<gfx::Vec> pos({ 0.0f, 0.0f }, window.get_center(), 2s, gfx::interpolators::ease_in_out_cubic);
    gfx::Animation<float> radius(0, 100, 2s);
    gfx::Animation<gfx::Color> color(gfx::Color::red(), gfx::Color::blue(), 5s);
    gfx::Animation<gfx::Rect> rect({ 300, 300, 100, 100 }, window.get_as_rect(), 1s, gfx::interpolators::ease_in_expo);

    pos.start();
    radius.start();
    color.start();
    rect.start();

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        rd.draw_rectangle(rect, gfx::Color::gray());
        rd.draw_circle(pos, radius, color);

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    });

}
