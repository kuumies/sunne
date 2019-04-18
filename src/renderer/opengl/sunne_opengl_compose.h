/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::OpenGLCompose class.
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
 * ---------------------------------------------------------------- */
class OpenGLCompose
{
public:
    OpenGLCompose();
    void draw();

    float exposure          = 0.9f;
    GLuint shadingTexMap    = 0;
    GLuint atmosphereTexMap = 0;
    GLuint starTexMap       = 0;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
