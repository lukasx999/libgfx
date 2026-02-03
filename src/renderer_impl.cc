#include <gfx/gfx.h>
#include <gfx/renderer.h>
#include "renderer_impl.h"

namespace gfx {

glm::mat4 Renderer::Impl::gen_view_matrix(const gfx::Surface& surface, gfx::Vec center, gfx::Rotation rotation, float scale) {
    gfx::Vec screen_center = surface.get_center();

    // the camera position used in the lookAt() function has its origin at (0,0), but
    // the origin of the viewer is the center of the screen, so we have to account for that
    // by subtracting the center coordinates
    glm::vec3 position(center.x - screen_center.x, center.y - screen_center.y, 0.0f);
    glm::vec3 direction(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(position, position+direction, up);

    // apply rotation
    view = glm::translate(view, glm::vec3(screen_center.x, screen_center.y, 0.0f));
    view = glm::rotate(view, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, glm::vec3(-screen_center.x, -screen_center.y, 0.0f));

    // apply scaling
    view = glm::translate(view, glm::vec3(screen_center.x, screen_center.y, 0.0f));
    view = glm::scale(view, glm::vec3(scale, scale, 0.0f));
    view = glm::translate(view, glm::vec3(-screen_center.x, -screen_center.y, 0.0f));

    return view;
}

} // namespace gfx
