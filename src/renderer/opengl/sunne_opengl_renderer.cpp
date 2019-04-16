/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLRenderer class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_renderer.h"
#include <glad/glad.h>
#include "sunne_opengl_compose.h"
#include "sunne_opengl_sun_render.h"

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
        , compose(std::make_shared<OpenGLCompose>())
        , sunRender(std::make_shared<OpenGLSunRender>(size))
    {}

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void resize(const glm::ivec2& newSize)
    {
        size = newSize;
        sunRender->resize(newSize);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void render()
    {
        sunRender->draw();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glViewport(0, 0, size.x, size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        compose->texMap = sunRender->tex;
        compose->draw();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::ivec2 size;
    std::shared_ptr<OpenGLCompose> compose;
    std::shared_ptr<OpenGLSunRender> sunRender;
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
void OpenGLRenderer::render()
{ impl->render(); }

} // namespace sunne
} // namespace kuu
