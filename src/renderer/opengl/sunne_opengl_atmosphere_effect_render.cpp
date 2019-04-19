/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   The implementation of kuu::sunne::OpenGLAtmosphereEffectRender class
 * ---------------------------------------------------------------- */
 
#include "sunne_opengl_atmosphere_effect_render.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "sunne_opengl_ndc_mesh.h"
#include "sunne_opengl_shader_loader.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLAtmosphereEffectRender::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(const glm::ivec2& size, OpenGLAtmosphereEffectRender* self)
        : size(size)
        , self(self)
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
                "shaders/sunne_opengl_atmosphere_effect_render.vsh",
                "shaders/sunne_opengl_atmosphere_effect_render.fsh");
        uniformViewport             = glGetUniformLocation(pgm, "viewport");
        uniformInverseView          = glGetUniformLocation(pgm, "inverseView");
        uniformInverseTransposeView = glGetUniformLocation(pgm, "inverseTransposeView");
        uniformInverseProjection    = glGetUniformLocation(pgm, "inverseProjection");
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
        ndcQuad = std::make_shared<NdcQuadMesh>(true);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyMesh()
    {
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
        glm::mat4 invProjection    = glm::inverse(scene.camera.projectionMatrix());
        glm::mat4 invView          = glm::inverse(scene.camera.viewMatrix());
        glm::mat3 invTransposeView = glm::inverseTranspose(glm::mat3(invView));

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glViewport(0, 0, size.x, size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(pgm);
        glUniform2f(uniformViewport, float(size.x), float(size.y));
        glUniformMatrix4fv(uniformInverseView,          1, GL_FALSE, glm::value_ptr(glm::mat4(invView)));
        glUniformMatrix4fv(uniformInverseProjection,    1, GL_FALSE, glm::value_ptr(glm::mat4(invProjection)));
        glUniformMatrix3fv(uniformInverseTransposeView, 1, GL_FALSE, glm::value_ptr(glm::mat3(invTransposeView)));

        ndcQuad->draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::ivec2 size;
    OpenGLAtmosphereEffectRender* self;
    GLuint rbo = 0;
    GLuint fbo = 0;
    GLuint pgm = 0;
    GLint uniformViewport;
    GLint uniformInverseView;
    GLint uniformInverseTransposeView;
    GLint uniformInverseProjection;
    std::shared_ptr<NdcQuadMesh> ndcQuad;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLAtmosphereEffectRender::OpenGLAtmosphereEffectRender(const glm::ivec2& size)
    : impl(std::make_shared<Impl>(size, this))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLAtmosphereEffectRender::resize(const glm::ivec2& size)
{ impl->resize(size); }

void OpenGLAtmosphereEffectRender::draw(const RendererScene& scene)
{ impl->draw(scene); }

} // namespace sunne
} // namespace kuu
