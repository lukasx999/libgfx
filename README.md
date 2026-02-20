# libgfx

libgfx is a 2D graphics library written in modern C++23.

## Goals

- An API that is easy to use right, and hard to use wrong
- Be cross-platform, portable and embeddable

## Platforms

*Only the currently implemented platforms have a checkmark next to them.*

Check out `examples/`, for on how to use libgfx with any of these technologies.

- [x] Linux via [glfw](https://www.glfw.org/)
- [x] Linux via Wayland & EGL
- [x] Linux via [gtkmm](https://gtkmm.gnome.org/en/index.html)
- [x] Web via [emscripten](https://emscripten.org/)
- [ ] Linux via [qt](https://www.qt.io/development/qt-framework)
- [ ] Windows via WinAPI

## Graphics APIs

libgfx currently supports the following graphics APIs:

- OpenGL 4.5 on desktop
- WebGL2 on the browser

## Use cases

- User Interfaces
- Data Visualizations
- Animations
- Games

## Build Instructions

### Requirements

- glfw3
- freetype2
- any C++23 compiler (clang/gcc)
- cmake

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

### Building for Web using [Emscripten](https://emscripten.org/)

```console
cd libgfx/
emcmake cmake -Bbuild
emmake make -C build -j$(nproc)
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
find_package(gfx REQUIRED)
```

And then link to the `gfx` target using `target_link_libraries()`.
