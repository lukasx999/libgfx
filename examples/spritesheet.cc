#include <gfx/gfx.h>

//
// Credits: https://kenmi-art.itch.io/cute-fantasy-rpg
//

int main() {

    gfx::Window window(700, 700, "hello world");

    gfx::Texture texture("spritesheet.png");
    gfx::Spritesheet spritesheet(texture, 32, 32);
    auto sprite = spritesheet.get(0, 0);

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        float scale = 2;
        gfx::Rect t {
            0.0f,
            0.0f,
            static_cast<float>(texture.get_width())*scale,
            static_cast<float>(texture.get_height())*scale,
        };
        rd.draw_texture(t, texture);
        rd.draw_texture(0, 0, 100, 100, sprite);

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    });

}
