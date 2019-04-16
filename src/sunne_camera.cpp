/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::Camera class.
 * ---------------------------------------------------------------- */

#include "sunne_camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
glm::mat4 Camera::viewMatrix() const
{
    glm::mat4 view(1.0f);
    view = glm::translate(view, position);
    view *= glm::mat4_cast(rotation);
    return glm::inverse(view);
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
glm::mat4 Camera::projectionMatrix() const
{
    return glm::perspective(
        glm::radians(fieldOfView),
        aspectRatio,
        nearPlane,
        farPlane);
}

} // namespace sunne
} // namespace kuu
