#include <gfx.h>

int main() {

    gfx::Window window(500, 500, "offscreen rendering");

    auto texture = window.draw_offscreen([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_circle(window.get_midpoint(), 100, gfx::Color::white());
    });

    texture.write_to_file(gfx::Texture::FileType::Png, "texture.png");
}
