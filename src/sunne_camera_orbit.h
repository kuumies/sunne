/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::CameraOrbit class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include "renderer/sunne_renderer_scene.h"
#include "window/sunne_window_user_input.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class CameraOrbit
{
public:
    CameraOrbit(std::shared_ptr<RendererScene::Camera> camera);

    void update(float elapsed);
    void setUserInput(const WindowUserInput& ui);
    void setTarget(std::shared_ptr<RendererScene::Satellite> target);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
