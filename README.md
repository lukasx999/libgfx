# libgfx

libgfx is an easy-to-use library for game/graphics programming written in modern C++23.

## Features

- Clean & Easy-To-Use Modern C++23 API
- Cross-Platform
- Embeddable into other applications

## Build Instructions

### Using [Just](https://github.com/casey/just)

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
#include <gfx/gfx.h>

int main() {

    gfx::Window window(1600, 900, "my gfx application");

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());
        rd.draw_rectangle(0, 0, 100, 100, gfx::Color::white());
    });

}
```

## Building Your Application

### Using CMake

This library comes with `find_package()` support for CMake, which means that after installing the library you can just insert this line into your `CMakeLists.txt`:

```cmake
find_package(gfx)
```

And then link to the `gfx` target using `target_link_libraries()`.
