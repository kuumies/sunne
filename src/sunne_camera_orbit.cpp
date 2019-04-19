/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::CameraOrbit class.
 * ---------------------------------------------------------------- */

#include "sunne_camera_orbit.h"
#include "window/sunne_window_user_input.h"
#include <iostream>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct CameraOrbit::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(CameraOrbit* self, RendererScene::Camera* camera)
        : self(self)
        , camera(camera)
    {}

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void update(float elapsed)
    {
        const float rotPerSecond = 2.0f;
        const float seconds = elapsed / 1000.0f;
        const float rotInc = seconds * rotPerSecond;
        glm::quat rot = glm::angleAxis(glm::radians(rotInc), glm::vec3(-1.0f, 0.0f, 0.0f));
        camera->rotation *= rot;
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    CameraOrbit* self;
    RendererScene::Camera* camera;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
CameraOrbit::CameraOrbit(RendererScene::Camera* camera)
    : impl(std::make_shared<Impl>(this, camera))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void CameraOrbit::update(float elapsed)
{ impl->update(elapsed); }

} // namespace sunne
} // namespace kuu
