# libgfx
easy to use c++ 2d graphics/game library

## Example


```c++
#include <gfx.hh>

int main() {

    gfx::Window window(1600, 900, "my window", gfx::WindowFlags::None);
    gfx::Renderer rd(window);

    rd.draw([&]() {
        rd.clear_background(gfx::Color::black());
        rd.draw_rectangle(0, 0, 100, 100, 0_deg, gfx::Color::red());
    });

}
```
