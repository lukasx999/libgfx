#include <functional>

#include <gtkmm.h>
#include <gfx/gfx.h>

class GfxArea : public Gtk::Frame {

    // using std::optional to defer construction of gfx context until
    // gktmm has initialized its opengl context.
    std::optional<gfx::ExternalContext> m_context;

    Gtk::GLArea m_area;

    using SignalRender = sigc::signal<void(gfx::Renderer&)>;
    SignalRender m_signal_render;

public:
    GfxArea() {
        m_area.set_required_version(4, 6);
        m_area.set_expand(true);
        m_area.set_auto_render(true);

        m_area.signal_resize().connect(sigc::mem_fun(*this, &GfxArea::on_gl_resize));
        m_area.signal_realize().connect(sigc::mem_fun(*this, &GfxArea::on_gl_realize));
        m_area.signal_unrealize().connect(sigc::mem_fun(*this, &GfxArea::on_gl_unrealize), false);
        m_area.signal_render().connect(sigc::mem_fun(*this, &GfxArea::on_gl_render), false);

        set_child(m_area);
    }

    [[nodiscard]] SignalRender& signal_on_render() {
        return m_signal_render;
    }

private:
    void on_gl_resize(int width, int height) {
        m_context.emplace(width, height);
    }

    bool on_gl_render([[maybe_unused]] const Glib::RefPtr<Gdk::GLContext>& context) {

        m_context->draw([&](gfx::Renderer& rd) {
            m_signal_render.emit(rd);
        });

        return true;
    }

    void on_gl_realize() {
        m_area.make_current();
    }

    void on_gl_unrealize() {
        m_context.reset();
    }

};

class MyWindow : public Gtk::Window {

    Gtk::Box m_box{Gtk::Orientation::VERTICAL, 0};
    GfxArea m_area;
    Gtk::Button m_button{"Hello, World!"};

public:
    MyWindow() {
        set_title("gtkmm example");
        set_default_size(1600, 900);

        m_area.signal_on_render().connect(sigc::mem_fun(*this, &MyWindow::draw));

        set_child(m_box);
        m_box.append(m_area);
        m_box.append(m_button);
    }

private:
    void draw(gfx::Renderer& rd) const {
        rd.clear_background(gfx::Color::white());
        rd.draw_circle(rd.get_surface().get_center(), 100, gfx::Color::blue());
    }

};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.example");
    return app->make_window_and_run<MyWindow>(argc, argv);
}
