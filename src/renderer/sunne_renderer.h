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
    virtual void resize(const glm::ivec2& size) = 0;
    virtual void render(const RendererScene& scene) = 0;
};

} // namespace sunne
} // namespace kuu
