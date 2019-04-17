/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLResources class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_resources.h"
#include <map>
#include <glad/glad.h>
#include "sunne_opengl_planet.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLResources::Impl
{
    std::map<std::string, std::shared_ptr<OpenGLPlanet>> planets;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLResources::OpenGLResources()
    : impl(std::make_shared<Impl>())
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
std::shared_ptr<OpenGLPlanet> OpenGLResources::openglPlanet(
    const RendererScene::Planet& planet)
{
    std::shared_ptr<OpenGLPlanet> out = impl->planets[planet.id];
    if (!out)
    {
        out = std::make_shared<OpenGLPlanet>(planet);
        impl->planets[planet.id] = out;
    }
    return out;
}

} // namespace sunne
} // namespace kuu
