/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::Renderer class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include <glm/vec2.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- */

class RendererScene;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    virtual void loadResources(std::shared_ptr<RendererScene> scene) = 0;
    virtual void resize(const glm::ivec2& size) = 0;
    virtual void render(std::shared_ptr<RendererScene> scene) = 0;
    virtual void renderResourceLoadWait() = 0;
};

} // namespace sunne
} // namespace kuu
