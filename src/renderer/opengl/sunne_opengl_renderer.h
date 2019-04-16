/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLRenderer class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include "../sunne_renderer.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(const glm::ivec2& size);
    virtual void resize(const glm::ivec2& size) override;
    virtual void render() override;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
