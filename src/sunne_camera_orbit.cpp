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

/*-----------------------------------------------------------------*/
// Calculates quaternion between two vectors so that v1*q = v2
// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
//
glm::quat quaternionBetween(
    const glm::vec3 &v1,
    const glm::vec3 &v2,
    bool normalizedVectors)
{
    // Calculate quaternion scalar part
    double w = 1;
    if (!normalizedVectors)
        w = sqrt(glm::length2(v1) * glm::length2(v2));
    w += glm::dot(v1, v2);

    // Calculate rotation axis
    glm::vec3 axis = glm::cross(v1, v2);

    // Construct quaternion and normalize it
    glm::quat rotation(w, axis);
    return glm::normalize(rotation);
}

float wrapAngleRadians(float angle)
{
    angle = fmod(angle, 2.0f * float(M_PI));
    if (angle < 0.0f)
        angle += 2.0f * float(M_PI);
    return angle;
}

float wrapAngleDegrees(float angle)
{
    return glm::degrees(wrapAngleRadians(glm::radians(angle)));
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct CameraOrbit::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(CameraOrbit* self, std::shared_ptr<RendererScene::Camera> camera)
        : self(self)
        , camera(camera)
    {
        target  = camera->rotation;
        target2 = camera->rotation2;
        p = glm::angleAxis(float(M_PI/3), glm::vec3(1.0f, 0.0f, 0.0f));
        pitch = glm::degrees(float(M_PI/3));
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void update(float elapsed)
    {
        static float totTime = 0.0f;
        totTime += elapsed;
//        std::cout << totTime << std::endl;
        // camera cut: 22550

        if (totTime < 22550)
        //if (false)
        {
            auto camPos = glm::vec3(glm::inverse(camera->viewMatrix()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            auto tgtPos = glm::vec3(targetSatellite->matrix()          * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
//            std::cout << glm::to_string(camPos) << " "
//                      << glm::to_string(tgtPos) << " "
//                      << glm::to_string(glm::normalize(camPos - tgtPos)) << std::endl;

            camera->doubleRot = false;
            camera->rotation = lookAt(glm::normalize(camPos - tgtPos), glm::vec3(0, 1, 0));

            if (focalLengthAnimation > 6000.0f)
                camera->lens.focalLength = glm::mix(14.0f, 180.0f, (focalLengthAnimation - 6000.0f) / 15000.0f);
            focalLengthAnimation += elapsed;
        }
        else
        {
            if (totTime < 45000.0f)
            //if (false)
            {
                static bool first = true;
                if (first)
                {
                    camera->position = glm::vec3(0.000000, 48.000000, 7048.000000);
                    camera->lens.focalLength = 40.0f;
                    first = false;
                }

                //const float rotPerSecond = 2.0f;
                //const float seconds = elapsed / 1000.0f;
                //const float rotInc = seconds * rotPerSecond;
                //target *= glm::angleAxis(glm::radians(rotInc), glm::vec3(-1.0f, 0.0f, 0.0f));

                //camera->rotation  = glm::slerp(camera->rotation,  target, 0.1f);

                auto tgtPos = glm::vec3(targetSatellite->matrix()          * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                auto camPos = glm::vec3(glm::inverse(camera->viewMatrix()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                camera->rotation = lookAt(glm::normalize(camPos - tgtPos), glm::vec3(0, 1, 0));
                camera->position = tgtPos + glm::vec3(10.0f, 10.0f, 10.0f);
                std::cout << glm::to_string(camera->position) << std::endl;
            }
            else //if (totTime < 59000.0f)
            {
                static bool first = true;
                if (first)
                {
                    camera->lens.focalLength = 60.0f;
                    camera->doubleRot = false;
                    first = false;
                }

                auto tgtPos = glm::vec3(targetSatellite->matrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                auto dir = glm::normalize(tgtPos - glm::vec3(0.0f));
                if (dir.z > 0.0f)
                    dir.z = -dir.z;
                std::cout << glm::to_string(dir) << std::endl;
                glm::quat rot = lookAt(dir, glm::vec3(0, 1, 0));
                camera->position = -dir * 21000.0f;
                camera->rotation = rot;
                //camera->rotation2 = glm::slerp(camera->rotation2, target2, 0.1f);
            }
        }
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void setUserInput(const WindowUserInput& ui)
    {
        if (ui.type == WindowUserInput::Type::Key)
        {
            float a = 0.1f;
            if (ui.key.key == GLFW_KEY_W)
                camera->position.z += a;
            if (ui.key.key == GLFW_KEY_S)
                camera->position.z -= a;
            if (ui.key.key == GLFW_KEY_D)
                camera->position.x += a;
            if (ui.key.key == GLFW_KEY_A)
                camera->position.x -= a;
            if (ui.key.key == GLFW_KEY_E)
                camera->position.y += a;
            if (ui.key.key == GLFW_KEY_Q)
                camera->position.y -= a;
            std::cout << glm::to_string(camera->position) << std::endl;
        }

        if (ui.type == WindowUserInput::Type::Mouse)
        {
            if (ui.mouse.status == GLFW_PRESS)
            {
                std::cout << "GLFW_PRESS" << std::endl;
                prevRotPos = ui.mouse.pos;
                rotate = true;
            }
            else if (ui.mouse.status == GLFW_RELEASE)
            {
                std::cout << "GLFW_RELEASE" << std::endl;
                prevRotPos = glm::vec2(0.0);
                rotate = false;
            }
        }
        else if (ui.type == WindowUserInput::Type::Cursor)
        {
            if (rotate)
            {
                std::cout << "rotate" << std::endl;
                glm::vec2 diff = ui.cursor.pos - prevRotPos;
                prevRotPos = ui.cursor.pos;
//                std::cout << glm::to_string(diff) << std::endl;

                yaw   += -diff.x * 0.5f;
                pitch += -diff.y * 0.5f;

                //const glm::vec3 yawAxis   = glm::vec3(0.0f, 1.0f, 0.0f);
                //y = glm::angleAxis(glm::radians(-yaw), yawAxis);
                //const glm::vec3 pitchAxis   = glm::vec3(0.0f, 1.0f, 0.0f);
                //p = glm::angleAxis(glm::radians(-yaw), yawAxis);

                //const glm::vec3 pitchAxis = glm::conjugate(y) * glm::vec3(1.0f, 0.0f, 0.0f);
                //p = glm::angleAxis(glm::radians(-pitch), pitchAxis);
                //p *= glm::angleAxis(glm::radians(-diff.x), yawAxis);
                //y *= glm::angleAxis(glm::radians(-diff.y), pitchAxis);
                target2 = p * y;

                //target2 *= /*glm::angleAxis(glm::radians(-diff.x), yawAxis) **/ glm::angleAxis(glm::radians(-diff.y), pitchAxis);


//                glm::quat yawRot   = glm::angleAxis(glm::radians(yaw),   glm::vec3(0, 1, 0));
//                glm::quat pitchRot = glm::angleAxis(glm::radians(pitch), yawRot * glm::vec3(1, 0, 0));
                //target2 = yawRot * pitchRot;

//                target2 = glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f));

                //eyeTargetRotation = mouseRotation;
//                glm::quat rot( glm::conjugate(target2) );
//                target2 *= glm::angleAxis( glm::radians(-diff.y / 5.0f), rot * glm::vec3(1,0,0));
//                target2 *= glm::angleAxis( glm::radians(-diff.x / 5.0f), glm::vec3(0,1,0));


//                    float rotateChangeAmount_ = 0.5f;

//                    const float yawChange   = -diff.x * rotateChangeAmount_;
//                    const float pitchChange = -diff.y * rotateChangeAmount_;

//                    glm::vec3 euler(pitch, yaw, 0.0f);
//                    euler.x += pitchChange;
//                    euler.y += yawChange;

//                    euler.x = wrapAngleDegrees(euler.x);
//                    euler.y = wrapAngleDegrees(euler.y);

//                    pitch = euler.x;
//                    yaw   = euler.y;

//                    const float pitch = glm::radians(pitchChange);
//                    const float yaw   = glm::radians(yawChange);

//                    glm::vec3 pitchAxis = glm::vec3(1.0f, 0.0f, 0.0f);
//                    glm::vec3 yawAxis   = glm::vec3(0.0f, 1.0f, 0.0f);
//                    yawAxis = target2 * yawAxis;

//                    target2 = glm::angleAxis(yaw,   yawAxis)   * target2;
//                    target2 = glm::angleAxis(pitch, pitchAxis) * target2;
            }
        }
        else if (ui.type == WindowUserInput::Type::Wheel)
        {
            camera->lens.focalLength += ui.wheel.pos.y;
            std::cout << camera->lens.focalLength << std::endl;
        }
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    CameraOrbit* self;
    std::shared_ptr<RendererScene::Camera> camera;
    std::shared_ptr<RendererScene::Satellite> targetSatellite;
    glm::quat target;
    glm::quat target2;
    float pitch = 0.0f, yaw = 0.0f;
    glm::quat p, y;
    bool rotate = false;
    glm::vec2 prevRotPos;
    float focalLengthAnimation = 0.0f;
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
void CameraOrbit::setUserInput(const WindowUserInput &ui)
{ impl->setUserInput(ui); }

void CameraOrbit::setTarget(std::shared_ptr<RendererScene::Satellite> target)
{ impl->targetSatellite = target; }

} // namespace sunne
} // namespace kuu
