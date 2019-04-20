/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   The implementation of kuu::sunne::OpenGLGeometryRender class
 * ---------------------------------------------------------------- */
 
#include "sunne_opengl_shading_render.h"
#include "sunne_opengl_planet.h"
#include "sunne_opengl_resources.h"
#include "sunne_opengl_satellite.h"
#include "../sunne_renderer_scene.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLShadingRender::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(const glm::ivec2& size,
         std::shared_ptr<OpenGLResources> resources,
         OpenGLShadingRender* self)
        : size(size)
        , self(self)
        , resources(resources)
    {
        createTexture();
        createRenderbuffer();
        createFramebuffer();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    ~Impl()
    {
        destroyFramebuffer();
        destroyRenderbuffer();
        destroyTexture();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createTexture()
    {
        glGenTextures(1, &self->tex);
        glBindTexture(GL_TEXTURE_2D, self->tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0,
                     GL_RGB16F, size.x, size.y, 0,
                     GL_RGB, GL_FLOAT, nullptr);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyTexture()
    {
        glDeleteTextures(1, &self->tex);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createRenderbuffer()
    {
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                              size.x, size.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyRenderbuffer()
    {
        glDeleteRenderbuffers(1, &rbo);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createFramebuffer()
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               self->tex,
                               0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  rbo);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyFramebuffer()
    {
        glDeleteFramebuffers(1, &fbo);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void resize(const glm::ivec2& newSize)
    {
        size = newSize;

        destroyTexture();
        destroyRenderbuffer();
        destroyFramebuffer();

        createTexture();
        createRenderbuffer();
        createFramebuffer();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void load(std::shared_ptr<RendererScene> scene)
    {
        for (std::shared_ptr<RendererScene::Planet> planet : scene->planets)
            resources->openglPlanet(planet)->loadResources();
        //resources->openglSatellite()->loadResources();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void draw(std::shared_ptr<RendererScene> scene)
    {
        const glm::mat4 view       = scene->camera->viewMatrix();
        const glm::mat4 projection = scene->camera->projectionMatrix();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glViewport(0, 0, size.x, size.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::shared_ptr<RendererScene::Planet> planet : scene->planets)
            resources->openglPlanet(planet)->draw(view, projection);
        resources->openglSatellite(scene->satellite)->draw(view, projection);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::ivec2 size;
    OpenGLShadingRender* self;
    GLuint rbo = 0;
    GLuint fbo = 0;
    std::shared_ptr<OpenGLResources> resources;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLShadingRender::OpenGLShadingRender(const glm::ivec2& size,
                                         std::shared_ptr<OpenGLResources> resources)
    : impl(std::make_shared<Impl>(size, resources, this))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLShadingRender::resize(const glm::ivec2& size)
{ impl->resize(size); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLShadingRender::load(std::shared_ptr<RendererScene> scene)
{ impl->load(scene); }

void OpenGLShadingRender::draw(std::shared_ptr<RendererScene> scene)
{ impl->draw(scene); }

} // namespace sunne
} // namespace kuu
