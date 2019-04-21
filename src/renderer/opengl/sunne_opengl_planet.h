/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLPlanet class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include <glad/glad.h>
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
    OpenGLPlanet(const glm::ivec2& size);

    void setPlanet(std::shared_ptr<RendererScene::Planet> planet);
    void loadResources();
    void resize(const glm::ivec2& size);
    void draw(const glm::mat4& view,
              const glm::mat4& projection);

    GLuint tex = 0;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
