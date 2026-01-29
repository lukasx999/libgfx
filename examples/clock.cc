#include <print>
#include <chrono>

#include <gfx/gfx.h>

namespace chrono = std::chrono;
using namespace std::chrono_literals;

class Clock {
    const std::string_view m_timezone;

    const gfx::Font& m_font;

    const int m_radius;
    const int m_hour_hand_length = m_radius * 0.4;
    const int m_minute_hand_length = m_radius * 0.7;
    const int m_second_hand_length = m_radius;
    static constexpr int m_center_point_radius = 3;
    static constexpr int m_index_fontsize = 30;
    static constexpr int m_index_number_spacing = 15;

    gfx::Animation<float> m_animation{0.0f, 1.0f, 1s, gfx::interpolators::ease_in_quad};

    template <typename T>
    struct Time {
        T hours, minutes, seconds;
    };

public:
    Clock(std::string_view timezone, const gfx::Font& font, int radius)
        : m_timezone(timezone)
        , m_font(font)
        , m_radius(radius)
    {
        m_animation.start();
    }

    void draw(gfx::Renderer& rd) const {
        rd.draw_circle(rd.get_surface().get_center(), m_radius, gfx::Color(20, 255));

        draw_indices_minutes(rd);
        draw_indices_hours(rd);

        if (m_animation.is_running()) {
            auto [hours, minutes, seconds] = get_hands_rotation_factors();

            draw_hand(rd, m_hour_hand_length, m_animation * hours);
            draw_hand(rd, m_minute_hand_length, m_animation * minutes);
            draw_hand(rd, m_second_hand_length, m_animation * seconds);

        } else {
            draw_hands(rd);
        }

        rd.draw_circle(rd.get_surface().get_center(), m_center_point_radius, gfx::Color::white());

    }

private:
    void draw_indices_minutes(gfx::Renderer& rd) const {
        draw_indices(rd, 25, 60, false);
    }

    void draw_indices_hours(gfx::Renderer& rd) const {
        draw_indices(rd, 50, 12, true);
    }

    void draw_indices(gfx::Renderer& rd, int length, int segments, bool draw_numbers) const {

        gfx::Vec center = rd.get_surface().get_center();
        gfx::Vec hand(0.0, -1.0);

        for (int segment = 1; segment <= segments; ++segment) {

            gfx::Vec direction = hand.rotated(gfx::Degrees(360.0 / segments * segment));
            gfx::Vec start = center + direction * (m_radius - length);
            gfx::Vec end = start + direction * length;

            rd.draw_line(start, end, gfx::Color::gray());

            if (draw_numbers) {
                gfx::Vec text_pos = center + direction * (m_radius + m_index_fontsize/2.0f + m_index_number_spacing);
                rd.draw_text_centered(text_pos, m_index_fontsize, std::to_string(segment), m_font, gfx::Color::white());
            }
        }

    }

    [[nodiscard]] Time<double> get_hands_rotation_factors() const {

        auto time = get_current_time();

        // add the remaining floating-point digits to hours and minutes, to
        // create a more precise representation
        double hours = time.hours + time.minutes / 60.0 + time.seconds / 3600.0;
        double minutes = time.minutes + time.seconds / 60.0;

        return {
            hours / 12.0,
            minutes / 60.0,
            time.seconds / 60.0,
        };

    }

    void draw_hands(gfx::Renderer& rd) const {

        auto [hours, minutes, seconds] = get_hands_rotation_factors();

        draw_hand(rd, m_hour_hand_length, hours);
        draw_hand(rd, m_minute_hand_length, minutes);
        draw_hand(rd, m_second_hand_length, seconds);

    }

    void draw_hand(gfx::Renderer& rd, int length, float rotation_factor) const {

        gfx::Vec center = rd.get_surface().get_center();
        gfx::Vec hand(0.0, -1.0);

        gfx::Vec direction = hand.rotated(gfx::Degrees(360.0 * rotation_factor));
        rd.draw_line(center, center + direction * length, gfx::Color::white());
    }

    [[nodiscard]] Time<long> get_current_time() const {

        chrono::zoned_time zone(m_timezone, chrono::system_clock::now());
        auto now = zone.get_local_time().time_since_epoch();

        auto hours = chrono::duration_cast<chrono::hours>(now) % 12;
        auto minutes = chrono::duration_cast<chrono::minutes>(now) % 60;
        auto seconds = chrono::duration_cast<chrono::seconds>(now) % 60;

        return {
            hours.count(),
            minutes.count(),
            seconds.count(),
        };
    }

};

int main() {

    gfx::Window window(700, 700, "clock");
    auto font = window.load_font("/usr/share/fonts/TTF/Roboto-Light.ttf");

    Clock clock("Europe/Vienna", font, 300);

    window.draw_loop([&](gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::black());

        clock.draw(rd);

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    });

}
