/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLPlanet class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include "../sunne_renderer_scene.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLPlanet
{
public:
    OpenGLPlanet(std::shared_ptr<RendererScene::Planet> planet);

    void loadResources();
    void draw(const glm::mat4& view,
              const glm::mat4& projection);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
