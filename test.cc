#include <gfx/gfx.h>

consteval void test_vec() {
    static_assert(gfx::Vec(3, 7) + gfx::Vec(7, 3) == gfx::Vec(10, 10));
    static_assert(gfx::Vec(4, 5) - gfx::Vec(5, 4) == gfx::Vec(-1, 1));
    static_assert(gfx::Vec(2, 9) * gfx::Vec(1, 5) == 2*1 + 9*5);
    static_assert(gfx::Vec(2, 9) * 0.5 == gfx::Vec(1, 4.5));
}

consteval void test_color() {

    constexpr auto a = gfx::Color::red();
    constexpr auto b = gfx::Color::blue();

    static_assert(gfx::Color(255, 255, 255, 255) == 0xffffffff);

    static_assert(gfx::lerp(a, b, 1.0f) == b);
    static_assert(gfx::lerp(a, b, 0.5f) == 0x7f007fff);
    static_assert(gfx::lerp(a, b, 0.0f) == a);

    static_assert(a * 0.0f == 0x00000000);
    static_assert(a * 1.0f == a);
    static_assert(a * 0.5f == 0x7f00007f);
}

consteval void test_rect() {

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

int main() {
    test_color();
    test_rect();
}
