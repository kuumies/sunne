/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   The implementation of kuu::sunne::OpenGLGeometryRender class
 * ---------------------------------------------------------------- */
 
#include "sunne_opengl_shading_render.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "sunne_opengl_ndc_mesh.h"
#include "sunne_opengl_planet.h"
#include "sunne_opengl_resources.h"
#include "sunne_opengl_shader_loader.h"
#include "sunne_opengl_sphere.h"
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
        createShader();
        createMesh();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    ~Impl()
    {
        destroyMesh();
        destroyShader();
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
    void createShader()
    {
        pgm = opengl_shader_loader::load(
                "shaders/sunne_opengl_shading_render.vsh",
                "shaders/sunne_opengl_shading_render.fsh");
        uniformProjectionMatrix = glGetUniformLocation(pgm, "matrices.projection");
        uniformViewMatrix       = glGetUniformLocation(pgm, "matrices.view");
        uniformModelMatrix      = glGetUniformLocation(pgm, "matrices.model");
        uniformNormalMatrix     = glGetUniformLocation(pgm, "matrices.normal");
        uniformAlbedoMap        = glGetUniformLocation(pgm, "albedoMap");
        uniformNormalMap        = glGetUniformLocation(pgm, "normalMap");
        uniformSpecularMap      = glGetUniformLocation(pgm, "specularMap");
        uniformCloudMap         = glGetUniformLocation(pgm, "cloudMap");
        uniformNightMap         = glGetUniformLocation(pgm, "nightMap");
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyShader()
    {
        glDeleteProgram(pgm);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createMesh()
    {
        ndcQuad = std::make_shared<NdcQuadMesh>();
        sphere  = std::make_shared<OpenGLSphere>(1.0f);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyMesh()
    {
        sphere.reset();
        ndcQuad.reset();
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
    void draw(const RendererScene& scene)
    {
        const glm::mat4 projectionMatrix = scene.camera.projectionMatrix();
        const glm::mat4 viewMatrix       = scene.camera.viewMatrix();
        const glm::mat4 modelMatrix      = glm::mat4(1.0f);
        const glm::mat3 normalMatrix     = glm::mat3(glm::inverseTranspose(modelMatrix));

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glViewport(0, 0, size.x, size.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(pgm);
        glUniformMatrix4fv(uniformModelMatrix, 1,
                           GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(uniformViewMatrix, 1,
                           GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(uniformProjectionMatrix, 1,
                           GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix3fv(uniformNormalMatrix, 1,
                           GL_FALSE, glm::value_ptr(normalMatrix));
        glUniform1i(uniformAlbedoMap,   0);
        glUniform1i(uniformNormalMap,   1);
        glUniform1i(uniformSpecularMap, 2);
        glUniform1i(uniformCloudMap,    3);
        glUniform1i(uniformNightMap,    4);

        for (const RendererScene::Planet& planet : scene.planets)
        {
            std::shared_ptr<OpenGLPlanet> glPlanet = resources->openglPlanet(planet);
            glPlanet->draw();

        }

        //sphere->draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::ivec2 size;
    OpenGLShadingRender* self;
    GLuint rbo = 0;
    GLuint fbo = 0;
    GLuint pgm = 0;
    GLint uniformProjectionMatrix;
    GLint uniformViewMatrix;
    GLint uniformModelMatrix;
    GLint uniformNormalMatrix;
    GLint uniformAlbedoMap;
    GLint uniformNormalMap;
    GLint uniformSpecularMap;
    GLint uniformCloudMap;
    GLint uniformNightMap;
    std::shared_ptr<NdcQuadMesh> ndcQuad;
    std::shared_ptr<OpenGLSphere> sphere;
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

void OpenGLShadingRender::draw(const RendererScene& scene)
{ impl->draw(scene); }

} // namespace sunne
} // namespace kuu
