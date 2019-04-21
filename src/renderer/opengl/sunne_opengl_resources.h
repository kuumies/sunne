/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLResources class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include "../sunne_renderer_scene.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- */

//class OpenGLPlanet;
class OpenGLSatellite;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLResources
{
public:
    OpenGLResources();

//    std::shared_ptr<OpenGLPlanet> openglPlanet(
//        std::shared_ptr<RendererScene::Planet> planet,
//        const glm::ivec2& size);
    std::shared_ptr<OpenGLSatellite> openglSatellite(
        std::shared_ptr<RendererScene::Satellite> satellite);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
