/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::RendererScene class.
 * ---------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class RendererScene
{
public:
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    class Camera
    {
    public:
        struct Lens
        {
            float focalLength    = 42.0f;    // mm
            float focusDistance  = 20000.0f; // mm
            float fStop          = 1.0f;     // f/N
            glm::ivec2 filmSize  = glm::ivec2(28.0f, 21.0f); // mm
            bool debug           = false;

            float fieldOfView() const
            {
                return glm::degrees(
                    2.0f * std::atan(
                        0.5f *  filmSize.y / focalLength));
            }
        };

        glm::mat4 viewMatrix() const;
        glm::mat4 projectionMatrix() const;

        glm::vec3 position  = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::quat rotation  = glm::quat();
        glm::quat rotation2 = glm::angleAxis(float(M_PI/3), glm::vec3(1.0f, 0.0f, 0.0f));
        bool doubleRot = false;

        float fieldOfView = 45.0f;
        float aspectRatio = 1.0f;
        float nearPlane   = 0.1f;
        float farPlane    = 150.0f;
        Lens lens;
    };

    /* ------------------------------------------------------------ *
       An initively far star is a directional light source.
     * ------------------------------------------------------------ */
    struct Star
    {
        std::string id;
        float radius;
        glm::vec3 intensity;
        glm::vec3 direction;
    };

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    struct Planet
    {
        std::string id;
        float distance;          // km
        float radius;            // km
        float inclination;       // degrees
        std::string albedoMap;
        std::string specularMap;
        std::string normalMap;
        std::string cloudMap;
        std::string nightMap;
        bool rotate = false;
    };

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    struct Satellite
    {
        glm::mat4 matrix() const;
        glm::vec3 position;
        glm::quat rotation;
    };

    // Constructs the default scene with sun and earth, camera is
    // observing the planet from outside.
    RendererScene();

    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<Star>> stars;
    std::vector<std::shared_ptr<Planet>> planets;
    std::shared_ptr<Satellite> satellite;
};

} // namespace sunne
} // namespace kuu
