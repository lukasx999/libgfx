#include <print>
#include <cassert>
#include <cstring>

#include <gfx.h>

using gfx::Vec;

int main() {

    Vec v1(1, 1);
    Vec v2(1, 1);
    assert(v1 - v2 == Vec(0, 0));
    assert(v1 + v2 == Vec(2, 2));

    gfx::Window window(500, 500, "test", gfx::WindowFlags::None);

}
