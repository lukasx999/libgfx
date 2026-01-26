#include <gfx/gfx.h>

int main() {

    gfx::Window window(500, 500, "offscreen rendering");

    auto texture = window.draw_offscreen([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_rectangle(0, 0, window.get_width(), window.get_height()/2.0, gfx::Color::white());
        rd.draw_circle(window.get_midpoint(), 150, gfx::Color::red());
    });

    texture.write_to_file(gfx::Texture::FileType::Png, "texture.png");

}
