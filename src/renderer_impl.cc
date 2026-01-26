#include <gfx/gfx.h>
#include <gfx/renderer.h>
#include "renderer_impl.h"

namespace gfx {

// TODO: zoom
// TODO: provide rectangle instead of center
glm::mat4 Renderer::Impl::gen_view_matrix(const gfx::Surface& surface, gfx::Vec center) {
    glm::vec3 camera_position(
        center.x - surface.get_width() / 2.0f,
        center.y - surface.get_height() / 2.0f,
        0.0f
    );

    glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    // TODO: center rotation
    // glm::mat4 transform(1.0f);
    // transform = glm::rotate(transform, gfx::deg_to_rad(10), glm::vec3(0.0f, 0.0f, 1.0f));
    // up = transform * glm::vec4(up, 1.0);

    glm::mat4 view = glm::lookAt(camera_position, camera_position+camera_direction, up);
    return view;
}

} // namespace gfx
