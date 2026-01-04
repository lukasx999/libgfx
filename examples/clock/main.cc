#include <chrono>

#include <gfx.h>

int main() {

    auto time = std::chrono::system_clock::now();

    gfx::Window window(900, 900, "Game", gfx::WindowFlags::Logging);
    gfx::Renderer rd(window);

    rd.draw([&] {

        rd.clear_background(gfx::Color::black());

        gfx::Vec center(window.get_width()/2.0, window.get_height()/2.0);

        rd.draw_circle(center, center.x, gfx::Color::gray());

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
