# libgfx

libgfx is an easy-to-use library for game/graphics programming written in modern C++23.

## Features

- Clean & Easy-to-use modern C++23 API
- Cross-Platform
- Embeddable into other applications

## Example


```c++
#include <gfx.h>

int main() {

    gfx::Window window(1600, 900, "my gfx application");
    gfx::Renderer renderer(window);

    renderer.draw_loop([&] {
        renderer.clear_background(gfx::Color::black());
        renderer.draw_rectangle(0, 0, 100, 100, gfx::Color::white());
    });

}
```
