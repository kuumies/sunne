/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLPlanet class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_planet.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include "sunne_opengl_shader_loader.h"
#include "sunne_opengl_texture_loader.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
#define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))

using namespace glm;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLPlanet::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(OpenGLPlanet* self, const ivec2& size)
        : self(self)
        , size(size)
    {
        createShader();
        createTexture();
        createRenderbuffer();
        createFramebuffer();
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
    void createMeshVao()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              14 * sizeof(float),
                              BUFFER_OFFSET(0));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                              14 * sizeof(float),
                              BUFFER_OFFSET(3 * sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                              14 * sizeof(float),
                              BUFFER_OFFSET(5 * sizeof(float)));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                              14 * sizeof(float),
                              BUFFER_OFFSET(8 * sizeof(float)));
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
                              14 * sizeof(float),
                              BUFFER_OFFSET(11 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createMeshBuffers()
    {
        struct Vertex
        {
            vec3 pos;
            vec2 texCoord;
            vec3 normal;
            vec3 tangent;
            vec3 bitangent;
        };

        const int ringCount   = 128;
        const int sectorCount = 128;
        const float pi        = float(M_PI);
        const float half_pi   = pi * 0.5f;
        const float radius    = planet->radius;

        std::vector<Vertex> vertexData;
        std::vector<unsigned> indexData;
        unsigned vertexCount = 0;

        auto addVertex = [&](const Vertex& v)
        {
            vertexData.push_back(v);
            indexData.push_back(vertexCount);
            vertexCount++;
        };

        auto addTriangle = [&](
                const Vertex& a,
                const Vertex& b,
                const Vertex& c)
        {
            addVertex(a);
            addVertex(b);
            addVertex(c);
        };

        auto addQuad = [&](
                const Vertex& a,
                const Vertex& b,
                const Vertex& c,
                const Vertex& d)
        {
            addTriangle(a, d, c);
            addTriangle(c, b, a);
        };

        float ringStep   = 1.0f / float(ringCount   - 1);
        float sectorStep = 1.0f / float(sectorCount - 1);

        std::vector<Vertex> vertices2;
        for(int r = 0; r < ringCount; ++r)
            for( int s = 0; s < sectorCount; ++s)
            {
                float y = sin(-half_pi + pi * r * ringStep);
                float x = cos(2 * pi * s * sectorStep) * sin(pi * r * ringStep);
                float z = sin(2 * pi * s * sectorStep) * sin(pi * r * ringStep);

                glm::vec3 p = glm::vec3(x, y, z) * float(radius);
                glm::vec2 tc = glm::vec2(s * sectorStep, r * ringStep);
                glm::vec3 n = glm::normalize(p - glm::vec3(0.0f));
                glm::vec3 t;
                glm::vec3 bt;

                vertices2.push_back(
                { p, tc, n, t, bt });
            }
        //unsigned vertexCount = 0;

        for(int r = 0; r < ringCount - 1; r++)
        {
            for(int s = 0; s < sectorCount - 1; s++)
            {
                size_t ia = size_t((r+0) * sectorCount + (s+0));
                size_t ib = size_t((r+0) * sectorCount + (s+1));
                size_t ic = size_t((r+1) * sectorCount + (s+1));
                size_t id = size_t((r+1) * sectorCount + (s+0));

                addQuad(vertices2[id],
                        vertices2[ic],
                        vertices2[ib],
                        vertices2[ia]);
            }
        }

        indexCount = GLsizei(indexData.size());

        for (size_t i = 0; i < indexData.size(); i += 3)
        {
            Vertex& v1 = vertexData[indexData[i + 0]];
            Vertex& v2 = vertexData[indexData[i + 1]];
            Vertex& v3 = vertexData[indexData[i + 2]];

            glm::dvec3 edge1 = v2.pos - v1.pos;
            glm::dvec3 edge2 = v3.pos - v1.pos;
            glm::dvec2 dUV1 = v2.texCoord - v1.texCoord;
            glm::dvec2 dUV2 = v3.texCoord - v1.texCoord;

            double f = 1.0 / (dUV1.x * dUV2.y -
                              dUV2.x * dUV1.y);

            glm::dvec3 tangent;
            tangent.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
            tangent.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
            tangent.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            glm::dvec3 bitangent;
            bitangent.x = f * (-dUV2.x * edge1.x + dUV1.x * edge2.x);
            bitangent.y = f * (-dUV2.x * edge1.y + dUV1.x * edge2.y);
            bitangent.z = f * (-dUV2.x * edge1.z + dUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);

            v1.tangent = tangent;
            v2.tangent = tangent;
            v3.tangent = tangent;

            v1.bitangent = bitangent;
            v2.bitangent = bitangent;
            v3.bitangent = bitangent;
        }

        std::vector<float> vertexData2;
        for (const Vertex& v : vertexData)
        {
            vertexData2.push_back(v.pos.x);
            vertexData2.push_back(v.pos.y);
            vertexData2.push_back(v.pos.z);
            vertexData2.push_back(v.texCoord.x);
            vertexData2.push_back(v.texCoord.y);
            vertexData2.push_back(v.normal.x);
            vertexData2.push_back(v.normal.y);
            vertexData2.push_back(v.normal.z);
            vertexData2.push_back(v.tangent.x);
            vertexData2.push_back(v.tangent.y);
            vertexData2.push_back(v.tangent.z);
            vertexData2.push_back(v.bitangent.x);
            vertexData2.push_back(v.bitangent.y);
            vertexData2.push_back(v.bitangent.z);
        }

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     GLsizeiptr(vertexData2.size() * sizeof(float)),
                     vertexData2.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     GLsizeiptr(indexData.size() * sizeof(unsigned int)),
                     indexData.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createTextures()
    {
        texNight    = opengl_texture_loader::load(planet->nightMap,    4, true);
        texCloud    = opengl_texture_loader::load(planet->cloudMap,    4, false);
        texAlbedo   = opengl_texture_loader::load(planet->albedoMap,   3, true);
        texNormal   = opengl_texture_loader::load(planet->normalMap,   3, false);
        texSpecular = opengl_texture_loader::load(planet->specularMap, 4, false);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createShader()
    {
        pgm = opengl_shader_loader::load(
                "shaders/sunne_opengl_planet.vsh",
                "shaders/sunne_opengl_planet.fsh");
        uniformProjectionMatrix       = glGetUniformLocation(pgm, "matrices.projection");
        uniformViewMatrix             = glGetUniformLocation(pgm, "matrices.view");
        uniformModelMatrix            = glGetUniformLocation(pgm, "matrices.model");
        uniformNormalMatrix           = glGetUniformLocation(pgm, "matrices.normal");
        uniformAlbedoMap              = glGetUniformLocation(pgm, "albedoMap");
        uniformNormalMap              = glGetUniformLocation(pgm, "normalMap");
        uniformSpecularMap            = glGetUniformLocation(pgm, "specularMap");
        uniformCloudMap               = glGetUniformLocation(pgm, "cloudMap");
        uniformNightMap               = glGetUniformLocation(pgm, "nightMap");
        uniformCloudMapTexCoordOffset = glGetUniformLocation(pgm, "cloudMapTexCoordOffset");
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void destroyShader()
    {
        glDeleteProgram(pgm);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    ~Impl()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        destroyShader();
        destroyFramebuffer();
        destroyRenderbuffer();
        destroyTexture();
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
    void loadResources()
    {
        createTextures();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void draw(const mat4& viewMatrix, const mat4& projectionMatrix)
    {
        if (vao == 0)
        {
            createMeshBuffers();
            createMeshVao();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glViewport(0, 0, size.x, size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texAlbedo);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texSpecular);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texCloud);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, texNight);

        // Inclination rotation
        glm::quat inclination =
            glm::angleAxis(glm::radians(planet->inclination),
                                        glm::vec3(0.0f, 0.0f, 1.0f));

        // Slow rotation
        float amount = 0.0005f;
        glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
        if (planet->rotate)
        {
            // Fast rotation
            amount = 0.05f;
            axis = planet->rotateAxis;
        }
        rotation *= glm::angleAxis(glm::radians(amount), axis);

        const glm::mat4 modelMatrix  = glm::mat4_cast(inclination * rotation);
        const glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(modelMatrix));

        // Update texture coordinate offset of clouds
        texOffset.x -= 0.00001f; // note: v-sync is enabled
        if (texOffset.x > 1.0f)
            texOffset.x = 1.0f - texOffset.x;
        if (texOffset.x < 0.0f)
            texOffset.x = 1.0f + texOffset.x;

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
        glUniform2fv(uniformCloudMapTexCoordOffset, 1,
                     glm::value_ptr(texOffset));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    OpenGLPlanet* self;
    std::shared_ptr<RendererScene::Planet> planet;
    ivec2 size;
    GLuint rbo = 0;
    GLuint fbo = 0;
    GLuint vao = 0;
    GLuint vbo;
    GLuint ibo;
    GLsizei indexCount;
    GLuint texAlbedo;
    GLuint texNormal;
    GLuint texSpecular;
    GLuint texCloud;
    GLuint texNight;
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
    GLint uniformCloudMapTexCoordOffset;
    glm::vec2 texOffset;
    glm::quat rotation;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLPlanet::OpenGLPlanet(const ivec2& size)
    : impl(std::make_shared<Impl>(this, size))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLPlanet::setPlanet(std::shared_ptr<RendererScene::Planet> planet)
{
    impl->planet = planet;
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLPlanet::loadResources()
{ impl->loadResources(); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLPlanet::resize(const ivec2& size)
{ impl->resize(size); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLPlanet::draw(const mat4& view, const mat4& projection)
{ impl->draw(view, projection); }

} // namespace sunne
} // namespace kuu
