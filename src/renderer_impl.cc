#include <gfx/gfx.h>
#include <gfx/renderer.h>
#include "renderer_impl.h"

namespace gfx {

// TODO: zoom
glm::mat4 Renderer::Impl::gen_view_matrix(const gfx::Surface& surface, gfx::Vec center, gfx::Rotation rotation) {
    gfx::Vec screen_center = surface.get_center();

    glm::vec3 camera_position(
        center.x - screen_center.x,
        center.y - screen_center.y,
        0.0f
    );

    glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(screen_center.x, screen_center.y, 0.0));
    transform = glm::rotate(transform, rotation.get_radians(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(-screen_center.x, -screen_center.y, 0.0));
    view = transform * view;

    return view;
}

} // namespace gfx
