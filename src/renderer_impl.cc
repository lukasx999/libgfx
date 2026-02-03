#include <gfx/gfx.h>
#include <gfx/renderer.h>
#include "renderer_impl.h"

namespace gfx {

// TODO: zoom
glm::mat4 Renderer::Impl::gen_view_matrix(const gfx::Surface& surface, gfx::Vec center, gfx::Rotation rotation) {
    gfx::Vec screen_center = surface.get_center();

    // the camera position used in the lookAt() function has its origin at (0,0), but
    // the origin of the viewer is the center of the screen, so we have to account for that
    // by subtracting the center coordinates
    glm::vec3 position(center.x - screen_center.x, center.y - screen_center.y, 0.0f);
    glm::vec3 direction(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(position, position+direction, up);

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(screen_center.x, screen_center.y, 0.0f));
    transform = glm::rotate(transform, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(-screen_center.x, -screen_center.y, 0.0f));
    view = transform * view;

    return view;
}

} // namespace gfx
