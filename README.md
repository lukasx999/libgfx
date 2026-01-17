# libgfx

easy to use c++ 2d graphics/game library

## Example


```c++
#include <gfx.h>

int main() {

    gfx::Window window(1600, 900, "my gfx application", gfx::WindowFlags::None);
    gfx::Renderer renderer(window);

    renderer.draw([&] {
        renderer.clear_background(gfx::Color::black());
        renderer.draw_rectangle(0, 0, 100, 100, gfx::Color::white());
    });

}
```
