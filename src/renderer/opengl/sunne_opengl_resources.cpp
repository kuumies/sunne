/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLResources class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_resources.h"
#include <map>
#include <glad/glad.h>
#include "sunne_opengl_planet.h"
#include "sunne_opengl_satellite.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLResources::Impl
{
    std::map<std::string, std::shared_ptr<OpenGLPlanet>> planets;
    std::shared_ptr<OpenGLSatellite> satellite;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLResources::OpenGLResources()
    : impl(std::make_shared<Impl>())
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
std::shared_ptr<OpenGLPlanet> OpenGLResources::openglPlanet(
    std::shared_ptr<RendererScene::Planet> planet)
{
    std::shared_ptr<OpenGLPlanet> out = impl->planets[planet->id];
    if (!out)
    {
        out = std::make_shared<OpenGLPlanet>(planet);
        impl->planets[planet->id] = out;
    }
    return out;
}

std::shared_ptr<OpenGLSatellite> OpenGLResources::openglSatellite(std::shared_ptr<RendererScene::Satellite> satellite)
{
    if (!impl->satellite)
    {
        impl->satellite = std::make_shared<OpenGLSatellite>(satellite);
        impl->satellite->loadResources();
    }
    return impl->satellite;
}

} // namespace sunne
} // namespace kuu
