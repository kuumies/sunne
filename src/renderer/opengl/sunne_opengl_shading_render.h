/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLShadingRender class
 * ---------------------------------------------------------------- */
 
#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace kuu
{
namespace sunne
{ 

/* ---------------------------------------------------------------- */

class OpenGLResources;
class RendererScene;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLShadingRender
{
public:
    OpenGLShadingRender(const glm::ivec2& size, std::shared_ptr<OpenGLResources> resources);
    void resize(const glm::ivec2& size);
    void load(const RendererScene& scene);
    void draw(const RendererScene& scene);

    GLuint tex = 0;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
