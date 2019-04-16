/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::Camera class.
 * ---------------------------------------------------------------- */

#pragma once

#include <glm/gtx/dual_quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class Camera
{
public:
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::quat rotation = glm::quat();

    float fieldOfView = 45.0f;
    float aspectRatio = 1.0f;
    float nearPlane   = 0.1f;
    float farPlane    = 150.0f;
};

} // namespace sunne
} // namespace kuu
