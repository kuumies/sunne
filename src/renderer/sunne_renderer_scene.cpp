/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::RendererScene class.
 * ---------------------------------------------------------------- */

#include "sunne_renderer_scene.h"
#include <iostream>
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
    view *= glm::mat4_cast(rotation);
    view = glm::translate(view, position);
    view *= glm::mat4_cast(glm::angleAxis(float(M_PI/3), glm::vec3(1.0f, 0.0f, 0.0f)));
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
glm::mat4 RendererScene::Satellite::matrix() const
{
    glm::mat4 r = glm::mat4_cast(rotation);
    glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    return r * t * s;
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
RendererScene::RendererScene()
{
    // Create sun
    std::shared_ptr<Star> sun = std::make_shared<Star>();
    sun->id     = "sun";
    sun->radius = 696342;
    stars.push_back(sun);

    // Create earth
    std::shared_ptr<Planet> earth = std::make_shared<Planet>();
    earth->id          = "earth";
    earth->distance    = 150000000;
    earth->radius      = 6371.0f;
    earth->inclination = 7.155f;
    //earth.albedoMap   = "textures/earth_albedo_day.png";
    //earth.normalMap   = "textures/earth_normal.png";
    //earth.specularMap = "textures/earth_specular.png";
    earth->albedoMap   = "textures/highres_earth_diffuse.png";
    earth->normalMap   = "textures/highres_earth_normal.png";
    earth->specularMap = "textures/highres_earth_spec.png";
    earth->cloudMap    = "textures/highres_earth_clouds.png";
    earth->nightMap    = "textures/highres_earth_night.png";
    planets.push_back(earth);

    // Camera
    const double atmosphereRadius = 6420;
    const double distance = atmosphereRadius * 1.1;
    camera = std::make_shared<Camera>();
    camera->position = glm::vec3(0.0f, 0.0f, distance);
    //camera.position = glm::vec3(0.0f, 0.0f, 10.0f);
    camera->farPlane = distance * 3;
    std::cout << distance << std::endl;

    // Satellite
    satellite = std::make_shared<Satellite>();
    satellite->position.y = 80;
    satellite->position.z = 7000;
    //satellite->rotation = glm::angleAxis(glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

} // namespace sunne
} // namespace kuu
