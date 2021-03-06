/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLAtmosphereEffectRender class
 * ---------------------------------------------------------------- */
 
#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include "../sunne_renderer_scene.h"

namespace kuu
{
namespace sunne
{ 

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLAtmosphereEffectRender
{
public:
    OpenGLAtmosphereEffectRender(const glm::ivec2& size);
    void resize(const glm::ivec2& size);
    void draw(std::shared_ptr<RendererScene> scene);

    GLuint tex = 0;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
