/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::opengl::sunne::opengl_shader_loader namespace.
 * ---------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

namespace kuu
{
namespace sunne
{
namespace opengl_shader_loader
{

GLuint load(const std::string& vshPath,
            const std::string& fshPath);

} // namespace opengl_shader_loader
} // namespace sunne
} // namespace kuu
