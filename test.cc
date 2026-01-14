#include <gfx.h>

int main() {

    {
        constexpr gfx::Rect a { 0, 0, 100, 100 };
        constexpr gfx::Rect b { 101, 0, 100, 100 };
        static_assert(!a.check_collision_rect(b));
    }

    {
        constexpr gfx::Rect a { 0, 0, 100, 100 };
        constexpr gfx::Rect b { 50, 50, 100, 100 };
        static_assert(a.check_collision_rect(b));
    }

}
