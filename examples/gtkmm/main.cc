#include <print>
#include <functional>

#include <gtkmm.h>
#include <gfx/gfx.h>

#include <epoxy/gl.h>

class MyWindow : public Gtk::Window {

    Gtk::GLArea m_glarea;
    Gtk::Button m_button{"Hello, World!"};
    Gtk::Box m_box{Gtk::Orientation::VERTICAL, 0};

    // using std::optional to defer construction of gfx context until
    // gktmm has initialized its opengl context.
    std::optional<gfx::ExternalContext> m_context;

public:
    MyWindow() {
        set_title("gtkmm example");
        set_default_size(1600, 900);

        m_glarea.set_required_version(4, 6);
        m_glarea.set_expand(true);
        m_glarea.set_auto_render(true);

        set_child(m_box);
        m_box.append(m_glarea);
        m_box.append(m_button);

        m_glarea.signal_resize().connect(sigc::mem_fun(*this, &MyWindow::on_gl_resize));
        m_glarea.signal_realize().connect(sigc::mem_fun(*this, &MyWindow::on_gl_realize));
        m_glarea.signal_unrealize().connect(sigc::mem_fun(*this, &MyWindow::on_gl_unrealize), false);
        m_glarea.signal_render().connect(sigc::mem_fun(*this, &MyWindow::on_gl_render), false);

    }

private:
    static void gfx_draw(gfx::Renderer& rd) {
        rd.clear_background(gfx::Color::white());
        rd.draw_circle(rd.get_surface().get_center(), 100, gfx::Color::blue());
    }

    void on_gl_resize(int width, int height) {
        m_context.emplace(width, height);
    }

    bool on_gl_render([[maybe_unused]] const Glib::RefPtr<Gdk::GLContext>& context) {
        m_glarea.throw_if_error();
        m_context->draw(gfx_draw);
        return true;
    }

    void on_gl_realize() {
        m_glarea.throw_if_error();
        m_glarea.make_current();
    }

    void on_gl_unrealize() {
        m_context.reset();
    }

};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.example");
    return app->make_window_and_run<MyWindow>(argc, argv);
}
