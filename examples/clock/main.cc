#include <chrono>
#include <print>

#include <gfx.h>

namespace {

class Clock {
    gfx::Renderer& m_rd;
    gfx::Window& m_window = m_rd.get_window();

    struct Time {
        int hour;
        int minute;
        int seconds;
    };

    const gfx::Vec m_center{ static_cast<float>(m_window.get_width()/2.0), static_cast<float>(m_window.get_height()/2.0) };
    const float m_radius = m_center.x;

public:
    explicit Clock(gfx::Renderer& rd) : m_rd(rd) { }

    void draw() {
        draw_body();
        draw_hands();
    }

private:
    void draw_body() {
        m_rd.draw_circle(m_center, m_radius, gfx::Color::gray());
    }

    void draw_hands() {

        float degree_per_hour = 360 / 12.0;
        float degree_per_minute = 360 / 60.0;
        float degree_per_seconds = 360 / 60.0;

        auto time = get_current_time();


        gfx::Vec hour_hand(0, -m_radius);
        gfx::Vec minute_hand(0, -m_radius);

        hour_hand.rotate(gfx::Degrees(degree_per_hour * time.hour));
        minute_hand.rotate(gfx::Degrees(degree_per_minute * time.minute));

        m_rd.draw_line(m_center, m_center+hour_hand, gfx::Color::red());
        m_rd.draw_line(m_center, m_center+minute_hand, gfx::Color::blue());
    }

    [[nodiscard]] static Time get_current_time() {

        namespace chrono = std::chrono;

        chrono::zoned_time zt("Europe/Vienna", chrono::system_clock::now());
        auto time = zt.get_local_time().time_since_epoch();

        auto hours = chrono::duration_cast<chrono::hours>(time) % 12;
        auto minutes = chrono::duration_cast<chrono::minutes>(time) % 60;
        auto seconds = chrono::duration_cast<chrono::seconds>(time) % 60;

        return {
            static_cast<int>(hours.count()),
            static_cast<int>(minutes.count()),
            static_cast<int>(seconds.count()),
        };
    }

};

} // namespace

int main() {


    gfx::Window window(900, 900, "Game", gfx::WindowFlags::Logging);
    gfx::Renderer rd(window);

    Clock clock(rd);

    rd.draw([&] {
        rd.clear_background(gfx::Color::black());

        clock.draw();

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

    });

}
