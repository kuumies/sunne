/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::CameraOrbit class.
 * ---------------------------------------------------------------- */

#include "sunne_camera_orbit.h"
#include "window/sunne_window_user_input.h"
#include <iostream>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>

namespace kuu
{
namespace sunne
{
namespace
{

/* ---------------------------------------------------------------- *
   See: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
 * ---------------------------------------------------------------- */
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
    using namespace glm;

    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = dot(start, dest);
    vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f){
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
        if (length2(rotationAxis) < 0.01f ) // bad luck, they were parallel, try again!
            rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = cross(start, dest);

    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

/* ---------------------------------------------------------------- *
   See: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
 * ---------------------------------------------------------------- */
glm::quat lookAt(glm::vec3 direction,
                 glm::vec3 desiredUp)
{
    using namespace glm;
    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    quat rot1 = RotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);

    // Recompute desiredUp so that it's perpendicular to the direction
    // You can skip that part if you really want to force desiredUp
    vec3 right = cross(direction, desiredUp);
    desiredUp = cross(right, direction);

    // Because of the 1rst rotation, the up is probably completely screwed up.
    // Find the rotation between the "up" of the rotated object, and the desired up
    vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
    quat rot2 = RotationBetweenVectors(newUp, desiredUp);

    quat targetOrientation = rot2 * rot1; // remember, in reverse order.
    return targetOrientation;
}

} // anonymous namespace

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct CameraOrbit::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(CameraOrbit* self, std::shared_ptr<RendererScene::Camera> camera)
        : self(self)
        , camera(camera)
    {}

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void update(float elapsed)
    {
        auto map = [](const glm::mat4& m)
        { return glm::vec3(m * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); };

        totTime += elapsed;

        const float cutA = 23550.0f;
        const float cutB = 46000.0f;
        const float cutC = 59000.0f;
        if (totTime < cutA)
        {
            auto camPos = map(glm::inverse(camera->viewMatrix()));
            auto tgtPos = map(targetSatellite->matrix());
            auto dir    = glm::normalize(camPos - tgtPos);
            camera->rotation = lookAt(dir, glm::vec3(0, 1, 0));

            // Run focal length "animation"
            const float start  = 6000.0f;
            const float end    = 15000.0f;
            const float fovMin = 14.0f;
            const float fovMax = 180.0f;

            if (focalLengthAnimation > start)
                camera->lens.focalLength = glm::mix(fovMin, fovMax, (focalLengthAnimation - start) / end);
            focalLengthAnimation += elapsed;
        }
        else if (totTime < cutB)
        {
            if (!cutSetB)
            {
                camera->position = glm::vec3(0.000000, 48.000000, 7048.000000);
                camera->lens.focalLength = 40.0f;
                cutSetB = true;
            }

            auto tgtPos = map(targetSatellite->matrix());
            auto camPos = map(glm::inverse(camera->viewMatrix()));
            auto dir    = glm::normalize(camPos - tgtPos);
            camera->rotation = lookAt(dir, glm::vec3(0, 1, 0));
            camera->position = tgtPos + glm::vec3(10.0f, 10.0f, 10.0f);
        }
        else if (totTime < cutC)
        {
            if (!cutSetC)
            {
                camera->position = glm::vec3(0.000000, 48.000000, 7048.000000);
                camera->lens.focalLength = 40.0f;
                cutSetC = true;
            }

            auto tgtPos = map(targetSatellite->matrix());
            auto camPos = map(glm::inverse(camera->viewMatrix()));
            auto dir    = glm::normalize(camPos - tgtPos);
            camera->rotation = lookAt(dir, glm::vec3(0, 1, 0));
            camera->position = tgtPos + glm::vec3(10.0f, 10.0f, -10.0f);
        }
        else
        {
            if (cutSetD)
            {
                camera->lens.focalLength = 60.0f;
                cutSetD = true;
            }

            auto tgtPos = map(targetSatellite->matrix());
            auto dir = glm::normalize(tgtPos - glm::vec3(0.0f));
            glm::quat rot = lookAt(dir, glm::vec3(0, 1, 0));
            camera->position = -dir * 21000.0f;
            camera->rotation = rot;
        }
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    CameraOrbit* self;
    std::shared_ptr<RendererScene::Camera> camera;
    std::shared_ptr<RendererScene::Satellite> targetSatellite;
    float focalLengthAnimation = 0.0f;
    float totTime = 0.0f;
    bool cutSetB = false;
    bool cutSetC = false;
    bool cutSetD = false;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
CameraOrbit::CameraOrbit(std::shared_ptr<RendererScene::Camera> camera)
    : impl(std::make_shared<Impl>(this, camera))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void CameraOrbit::update(float elapsed)
{ impl->update(elapsed); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void CameraOrbit::setTarget(std::shared_ptr<RendererScene::Satellite> target)
{ impl->targetSatellite = target; }

} // namespace sunne
} // namespace kuu
