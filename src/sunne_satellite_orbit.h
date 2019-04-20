/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::SatelliteOrbit class.
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
class SatelliteOrbit
{
public:
    SatelliteOrbit(std::shared_ptr<RendererScene::Satellite> satellite);

    void update(float elapsed);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
