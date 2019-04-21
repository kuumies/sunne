/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLRenderer class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_renderer.h"
#include <glad/glad.h>
#include "../sunne_renderer_scene.h"
#include "sunne_opengl_atmosphere_effect_render.h"
#include "sunne_opengl_compose.h"
#include "sunne_opengl_loading.h"
#include "sunne_opengl_planet.h"
#include "sunne_opengl_resources.h"
#include "sunne_opengl_shading_render.h"
#include "sunne_opengl_star_effect_render.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLRenderer::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(const glm::ivec2& size)
        : size(size)
    {
        resources        = std::make_shared<OpenGLResources>();
        loading          = std::make_shared<OpenGLLoading>();
        shading          = std::make_shared<OpenGLShadingRender>(size, resources);
        atmosphereEffect = std::make_shared<OpenGLAtmosphereEffectRender>(size);
        starEffect       = std::make_shared<OpenGLStarEffectRender>(size);
        planet           = std::make_shared<OpenGLPlanet>(size);
        compose          = std::make_shared<OpenGLCompose>();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void resize(const glm::ivec2& newSize)
    {
        size = newSize;
        shading->resize(newSize);
        atmosphereEffect->resize(newSize);
        starEffect->resize(newSize);
        planet->resize(newSize);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void loadResources(std::shared_ptr<RendererScene> scene)
    {
        planet->setPlanet(scene->planets.front());
        planet->loadResources();
        shading->load(scene);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void render(std::shared_ptr<RendererScene> scene)
    {
        shading->draw(scene);
        atmosphereEffect->draw(scene);
        planet->setPlanet(scene->planets.front());
        planet->draw(scene->camera->viewMatrix(),
                     scene->camera->projectionMatrix());
        starEffect->draw();
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 0.2f, 0.2f, 1.0f);
        glViewport(0, 0, size.x, size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        compose->shadingTexMap    = shading->tex;
        compose->atmosphereTexMap = atmosphereEffect->tex;
        compose->starTexMap       = starEffect->tex;
        compose->planetTexMap     = planet->tex;
        compose->draw();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void renderResourceLoadWait()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glViewport(0, 0, size.x, size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        loading->draw();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::ivec2 size;
    std::shared_ptr<OpenGLResources> resources;
    std::shared_ptr<OpenGLLoading> loading;
    std::shared_ptr<OpenGLShadingRender> shading;
    std::shared_ptr<OpenGLAtmosphereEffectRender> atmosphereEffect;
    std::shared_ptr<OpenGLStarEffectRender> starEffect;
    std::shared_ptr<OpenGLPlanet> planet;
    std::shared_ptr<OpenGLCompose> compose;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLRenderer::OpenGLRenderer(const glm::ivec2& size)
    : impl(std::make_shared<Impl>(size))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLRenderer::resize(const glm::ivec2& size)
{ impl->resize(size); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLRenderer::render(std::shared_ptr<RendererScene> scene)
{ impl->render(scene); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLRenderer::loadResources(std::shared_ptr<RendererScene> scene)
{ impl->loadResources(scene); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLRenderer::renderResourceLoadWait()
{ impl->renderResourceLoadWait(); }

} // namespace sunne
} // namespace kuu
