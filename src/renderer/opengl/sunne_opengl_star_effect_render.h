/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLStarEffectRender class
 * ---------------------------------------------------------------- */
 
#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace kuu
{
namespace sunne
{ 

/* ---------------------------------------------------------------- *
   Renders the sun effects into framebuffer that can be later
   used during composion.
 * ---------------------------------------------------------------- */
class OpenGLStarEffectRender
{
public:
    OpenGLStarEffectRender(const glm::ivec2& size);
    void resize(const glm::ivec2& size);
    void draw();

    GLuint tex = 0;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
