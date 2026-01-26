# libgfx

libgfx is an easy-to-use library for game/graphics programming written in modern C++23.

## Features

- Clean & Easy-to-use modern C++23 API
- Cross-Platform
- Embeddable into other applications

## Build Instructions

### If you have [Just](https://github.com/casey/just)

```console
cd libgfx/
sudo just install
```

### Using CMake

```console
cd libgfx/
cmake -Bbuild
cmake -Bbuild -GNinja # for faster builds using Ninja
cmake --build build
sudo cmake --install build
```

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
