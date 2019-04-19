/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   The implementation of kuu::sunne::OpenGLLoading class
 * ---------------------------------------------------------------- */
 
#include "sunne_opengl_loading.h"
#include "sunne_opengl_ndc_mesh.h"
#include "sunne_opengl_shader_loader.h"
#include "sunne_opengl_texture_loader.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLLoading::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(OpenGLLoading* self)
        : self(self)
    {
        createTexture();
        createShader();
        createMesh();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    ~Impl()
    {
        destroyMesh();
        destroyShader();
        destroyTexture();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createTexture()
    {
        tex = opengl_texture_loader::load("textures/loading.png", 4, false);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyTexture()
    {
        glDeleteTextures(1, &tex);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createShader()
    {
        pgm = opengl_shader_loader::load(
                "shaders/sunne_opengl_loading.vsh",
                "shaders/sunne_opengl_loading.fsh");
        uniformTex = glGetUniformLocation(pgm, "tex");
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
    void draw()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUseProgram(pgm);
        glUniform1i(uniformTex, 0);

        ndcQuad->draw();

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    OpenGLLoading* self;
    GLuint tex = 0;
    GLuint pgm = 0;
    GLint uniformTex = 0;
    std::shared_ptr<NdcQuadMesh> ndcQuad;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLLoading::OpenGLLoading()
    : impl(std::make_shared<Impl>(this))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLLoading::draw()
{ impl->draw(); }

} // namespace sunne
} // namespace kuu
