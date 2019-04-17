/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::RendererScene class.
 * ---------------------------------------------------------------- */

#include "sunne_renderer_scene.h"
#include <glm/gtc/matrix_transform.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
glm::mat4 RendererScene::Camera::viewMatrix() const
{
    glm::mat4 view(1.0f);
    view = glm::translate(view, position);
    view *= glm::mat4_cast(rotation);
    return glm::inverse(view);
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
glm::mat4 RendererScene::Camera::projectionMatrix() const
{
    return glm::perspective(
        glm::radians(fieldOfView),
        aspectRatio,
        nearPlane,
        farPlane);
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
RendererScene::RendererScene()
{
    // Create sun
    Star sun;
    sun.id     = "sun";
    sun.radius = 696342;
    stars.push_back(sun);

    // Create earth
    Planet earth;
    earth.id          = "earth";
    earth.distance    = 150000000;
    earth.radius      = 6371.0f;
    earth.inclination = 7.155f;
    earth.albedoMap   = "textures/earth_albedo_day.png";
    earth.normalMap   = "textures/earth_normal.png";
    earth.specularMap = "textures/earth_specular.png";
    planets.push_back(earth);
}

} // namespace sunne
} // namespace kuu
