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

class OpenGLPlanet;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLResources
{
public:
    OpenGLResources();

    std::shared_ptr<OpenGLPlanet> openglPlanet(
        const RendererScene::Planet& planet);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
