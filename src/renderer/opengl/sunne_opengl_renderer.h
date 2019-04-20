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
   Renders the scene into different framebuffers and then composes
   the final image from the framebuffer contents.

   The framebuffers are:

    1) geometry:   Contains the geometry of planets and sun with
                   shading from the texture maps. This is the master
                   framebuffer and could be used alone.

    2) star:       Contains the start effects ie. makes it sun look
                   like a distant star.

    3) atmosphere: Contains the atmosphere effects of the given in
                   planets. The planes are rendered one-by-one using
                   depthbuffer of geometry step. This does not use
                   geometry path parameteric values and ray casts.
 * ---------------------------------------------------------------- */
class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(const glm::ivec2& size);
    virtual void resize(const glm::ivec2& size) override;
    virtual void render(std::shared_ptr<RendererScene> scene) override;
    virtual void loadResources(std::shared_ptr<RendererScene> scene) override;
    virtual void renderResourceLoadWait() override;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
