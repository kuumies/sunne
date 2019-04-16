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

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    virtual void resize(const glm::ivec2& size) = 0;
    virtual void render() = 0;
};

} // namespace sunne
} // namespace kuu
