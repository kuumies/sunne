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
        uniformShadingTexMap    = glGetUniformLocation(pgm, "shadingTexMap");
        uniformAtmosphereTexMap = glGetUniformLocation(pgm, "atmosphereTexMap");
        uniformStarTexMap       = glGetUniformLocation(pgm, "starTexMap");
        uniformPlanetTexMap     = glGetUniformLocation(pgm, "planetTexMap");
        uniformExposure         = glGetUniformLocation(pgm, "exposure");
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
        glBindTexture(GL_TEXTURE_2D, self->shadingTexMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, self->atmosphereTexMap);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, self->starTexMap);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, self->planetTexMap);

        glUseProgram(pgm);
        glUniform1i(uniformShadingTexMap,    0);
        glUniform1i(uniformAtmosphereTexMap, 1);
        glUniform1i(uniformStarTexMap,       2);
        glUniform1i(uniformPlanetTexMap,     3);
        glUniform1f(uniformExposure,         self->exposure);

        ndcQuad->draw();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    OpenGLCompose* self;
    GLuint pgm;
    GLint uniformShadingTexMap;
    GLint uniformAtmosphereTexMap;
    GLint uniformStarTexMap;
    GLint uniformPlanetTexMap;
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
