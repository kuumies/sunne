/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLSatellite class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include "../sunne_renderer_scene.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLSatellite
{
public:
    OpenGLSatellite(std::shared_ptr<RendererScene::Satellite> satellite);

    void loadResources();
    void draw(const glm::mat4& view,
              const glm::mat4& projection);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
