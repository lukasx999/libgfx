#include <memory>
#include <functional>

#include <gfx/surface.h>
#include <gfx/renderer.h>

namespace gfx {

class WaylandWindow : public gfx::Surface {
public:
    using DrawFn = std::function<void(gfx::Renderer&)>;

    WaylandWindow(int width, int height, const char* title);
    ~WaylandWindow();
    WaylandWindow(const WaylandWindow&) = delete;
    WaylandWindow& operator=(const WaylandWindow&) = delete;

    [[nodiscard]] int get_width() const override;
    [[nodiscard]] int get_height() const override;
    void draw_loop(DrawFn draw_fn);

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    DrawFn m_draw_fn;
    std::optional<gfx::Renderer> m_renderer;

};

} // namespace gfx
