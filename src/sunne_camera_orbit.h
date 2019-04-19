/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::CameraOrbit class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include "renderer/sunne_renderer_scene.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class CameraOrbit
{
public:
    CameraOrbit(RendererScene::Camera* camera);

    void update(float elapsed);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
