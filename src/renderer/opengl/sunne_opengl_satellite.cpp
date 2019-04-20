/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLSatellite class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_satellite.h"
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
#include "../sunne_pbr_model_importer.h"

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
struct OpenGLSatellite::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    struct Mesh
    {
        ModelImporter::Model model;
        GLuint vao = 0;
        GLuint vbo;
        GLuint ibo;
        GLsizei indexCount;
        GLuint texAlbedo;
        GLuint texNormal;
        GLuint texSpecular;
        GLuint texCloud;
        GLuint texNight;
    };

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(std::shared_ptr<RendererScene::Satellite> satellite)
        : satellite(satellite)
    {
        createShader();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createShader()
    {
        pgm = opengl_shader_loader::load(
                "shaders/sunne_opengl_satellite.vsh",
                "shaders/sunne_opengl_satellite.fsh");
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
    void createMesh(Mesh& mesh)
    {
        std::vector<unsigned int> indexData = mesh.model.mesh->indices;
        mesh.indexCount = GLsizei(indexData.size());

        std::vector<float> vertexData;
        for (const ModelImporter::Vertex& v : mesh.model.mesh->vertices)
        {
            vertexData.push_back(v.position.x);
            vertexData.push_back(v.position.y);
            vertexData.push_back(v.position.z);
            vertexData.push_back(v.texCoord.x);
            vertexData.push_back(v.texCoord.y);
            vertexData.push_back(v.normal.x);
            vertexData.push_back(v.normal.y);
            vertexData.push_back(v.normal.z);
            vertexData.push_back(v.tangent.x);
            vertexData.push_back(v.tangent.y);
            vertexData.push_back(v.tangent.z);
            vertexData.push_back(v.bitangent.x);
            vertexData.push_back(v.bitangent.y);
            vertexData.push_back(v.bitangent.z);
        }

        glGenBuffers(1, &mesh.vbo);
        glGenBuffers(1, &mesh.ibo);

        std::cout << vertexData.size() << ", " << indexData.size() << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     GLsizeiptr(vertexData.size() * sizeof(float)),
                     vertexData.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     GLsizeiptr(indexData.size() * sizeof(unsigned int)),
                     indexData.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
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
    void loadModel()
    {
        meshes.clear();
        std::vector<ModelImporter::Model> models =
            ModelImporter().import("models/satellite/satellite.obj");
        for (ModelImporter::Model& model : models)
        {
            model.transform->position.z += 9600;

            Mesh mesh  = {};
            mesh.model = model;
            meshes.push_back(mesh);
        }
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createTextures(Mesh& mesh)
    {
        std::shared_ptr<ModelImporter::Material> material = mesh.model.material;
        if (!material)
            return;
        if (material->albedo.empty())
            return;
        mesh.texAlbedo   =opengl_texture_loader::load(material->albedo,   4, true);
//        texNight    = opengl_texture_loader::load(planet.nightMap,    4, true);
//        texCloud    = opengl_texture_loader::load(planet.cloudMap,    4, false);
//        texNormal   = opengl_texture_loader::load(planet.normalMap,   3, false);
//        texSpecular = opengl_texture_loader::load(planet.specularMap, 4, false);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    ~Impl()
    {
//        glDeleteVertexArrays(1, &vao);
//        glDeleteBuffers(1, &vbo);
//        glDeleteBuffers(1, &ibo);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void loadResources()
    {
        loadModel();
//        createTextures();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void draw(const glm::mat4& viewMatrix,
              const glm::mat4& projectionMatrix)
    {
        for (Mesh& mesh : meshes)
        {
            if (mesh.vao != 0)
                continue;

            createMesh(mesh);
            createTextures(mesh);
        }

        for (Mesh& mesh : meshes)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texAlbedo);
    //        glActiveTexture(GL_TEXTURE1);
    //        glBindTexture(GL_TEXTURE_2D, texNormal);
    //        glActiveTexture(GL_TEXTURE2);
    //        glBindTexture(GL_TEXTURE_2D, texSpecular);
    //        glActiveTexture(GL_TEXTURE3);
    //        glBindTexture(GL_TEXTURE_2D, texCloud);
    //        glActiveTexture(GL_TEXTURE4);
    //        glBindTexture(GL_TEXTURE_2D, texNight);

            const glm::mat4 modelMatrix  = satellite->matrix(); //mesh.model.transform->matrix();
            const glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(modelMatrix));


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
//            glUniform1i(uniformNormalMap,   1);
//            glUniform1i(uniformSpecularMap, 2);
//            glUniform1i(uniformCloudMap,    3);
//            glUniform1i(uniformNightMap,    4);

            glBindVertexArray(mesh.vao);
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    std::shared_ptr<RendererScene::Satellite> satellite;
    std::vector<Mesh> meshes;
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
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLSatellite::OpenGLSatellite(std::shared_ptr<RendererScene::Satellite> satellite)
    : impl(std::make_shared<Impl>(satellite))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLSatellite::loadResources()
{ impl->loadResources(); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLSatellite::draw(const glm::mat4& view,
                           const glm::mat4& projection)
{ impl->draw(view, projection); }

} // namespace sunne
} // namespace kuu
