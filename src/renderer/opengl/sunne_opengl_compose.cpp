/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::OpenGLCompose class.
 * ---------------------------------------------------------------- */
 
#include "sunne_opengl_compose.h"
#include "sunne_opengl_shader_loader.h"
#include "sunne_opengl_ndc_mesh.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLCompose::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(OpenGLCompose* self)
        : self(self)
    {
        createShader();
        createMesh();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    ~Impl()
    {
        deleteShader();
        deleteMesh();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createShader()
    {
        pgm = opengl_shader_loader::load(
            "shaders/sunne_opengl_compose.vsh",
            "shaders/sunne_opengl_compose.fsh");
        uniformTexMap   = glGetUniformLocation(pgm, "texMap");
        uniformExposure = glGetUniformLocation(pgm, "exposure");
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void deleteShader()
    {
        glDeleteProgram(pgm);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createMesh()
    {
        ndcQuad = std::make_shared<NdcQuadMesh>();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void deleteMesh()
    {
        ndcQuad.reset();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, self->texMap);

        glUseProgram(pgm);
        glUniform1i(uniformTexMap,   0);
        glUniform1f(uniformExposure, self->exposure);

        ndcQuad->draw();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    OpenGLCompose* self;
    GLuint pgm;
    GLint uniformTexMap;
    GLint uniformExposure;
    std::shared_ptr<NdcQuadMesh> ndcQuad;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLCompose::OpenGLCompose()
    : impl(std::make_shared<Impl>(this))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLCompose::draw()
{ impl->draw(); }

} // namespace sunne
} // namespace kuu
