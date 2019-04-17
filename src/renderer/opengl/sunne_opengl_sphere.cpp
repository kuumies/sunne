/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLSphere class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_sphere.h"
#include <math.h>
#include <vector>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>

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
struct OpenGLSphere::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(float radius)
    {
        struct Vertex
        {
            vec3 pos;
            vec2 texCoord;
            vec3 normal;
            vec3 tangent;
            vec3 bitangent;
        };

        const int ringCount   = 32;
        const int sectorCount = 32;
        const float pi        = float(M_PI);
        const float half_pi   = pi * 0.5f;

        const float ringStep   = 1.0f / float(ringCount   - 1);
        const float sectorStep = 1.0f / float(sectorCount - 1);

        std::vector<Vertex> vertexData;
        for(int r = 0; r < ringCount;   ++r)
        for(int s = 0; s < sectorCount; ++s)
        {
            float y = sin(half_pi + pi * r * ringStep);
            float x = cos(2.0f * pi * s * sectorStep) * sin(pi * r * ringStep);
            float z = sin(2.0f * pi * s * sectorStep) * sin(pi * r * ringStep);

            Vertex v;
            v.pos = vec3(x, y, z) * radius;
            v.texCoord = vec2(s * sectorStep, r * ringStep);
            v.normal = normalize(v.pos);

            vertexData.push_back(v);
        }

        std::vector<unsigned int> indexData;
        for(int r = 0; r < ringCount   - 1; r++)
        for(int s = 0; s < sectorCount - 1; s++)
        {
            unsigned ia = unsigned((r+0) * sectorCount + (s+0));
            unsigned ib = unsigned((r+0) * sectorCount + (s+1));
            unsigned ic = unsigned((r+1) * sectorCount + (s+1));
            unsigned id = unsigned((r+1) * sectorCount + (s+0));

            indexData.push_back(id);
            indexData.push_back(ia);
            indexData.push_back(ib);

            indexData.push_back(ib);
            indexData.push_back(ic);
            indexData.push_back(id);
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

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindVertexArray(vao);
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

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
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
    ~Impl()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void draw()
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLsizei indexCount;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLSphere::OpenGLSphere(float radius)
    : impl(std::make_shared<Impl>(radius))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLSphere::draw()
{
    impl->draw();
}

} // namespace sunne
} // namespace kuu
