/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLLoading class
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
class OpenGLLoading
{
public:
    OpenGLLoading();
    void draw();

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
