/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::opengl_texture_loader namespace.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>

namespace kuu
{
namespace sunne
{
namespace opengl_texture_loader
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
GLuint load(const std::string& path, int req_comp, bool sRgb);

} // namespace opengl_texture_loader
} // namespace sunne
} // namespace kuu
