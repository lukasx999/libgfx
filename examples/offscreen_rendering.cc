#include <gfx.h>

int main() {

    gfx::Window window(500, 500, "offscreen rendering");
    gfx::Renderer rd(window);

    auto texture = rd.to_texture([&] {
        rd.clear_background(gfx::Color::black());
        rd.draw_circle(window.get_midpoint(), 100, gfx::Color::white());
    });

    texture.write_to_file(gfx::Texture::FileType::Png, "texture.png");
}
