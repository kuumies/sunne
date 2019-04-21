/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   The implementation of kuu::sunne::ModelImporter class.
 * ---------------------------------------------------------------- */
 
#include "sunne_pbr_model_importer.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/geometric.hpp>
#include <stb_image.h>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct ModelImporter::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl()
    {}

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::dvec3 toVec3(const aiColor3D& c)
    { return glm::dvec3(c.r, c.g, c.b); }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    glm::dvec3 toVec3(const aiVector3D& v)
    { return glm::dvec3(v.x, v.y, v.z); }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    std::shared_ptr<Mesh> importMesh(const aiMesh* const mesh)
    {
        std::shared_ptr<Mesh> out = std::make_shared<Mesh>();

        for (size_t v = 0; v < mesh->mNumVertices; ++v)
        {
            Vertex vertex;

            if (mesh->HasPositions())
            {
                const aiVector3D* p = &mesh->mVertices[v];
                vertex.position.x = float(p->x);
                vertex.position.y = float(p->y);
                vertex.position.z = float(p->z);
            }

            if (mesh->HasTextureCoords(0))
            {
                const aiVector3D* tc = &mesh->mTextureCoords[0][v];
                vertex.texCoord.x = float(tc->x);
                vertex.texCoord.y = float(tc->y);
            }

            if (mesh->HasNormals())
            {
                const aiVector3D* n = &mesh->mNormals[v];
                vertex.normal.x = float(n->x);
                vertex.normal.y = float(n->y);
                vertex.normal.z = float(n->z);
            }

            if (mesh->HasTangentsAndBitangents())
            {
                const aiVector3D* t = &mesh->mTangents[v];
                vertex.tangent.x = float(t->x);
                vertex.tangent.y = float(t->y);
                vertex.tangent.z = float(t->z);

                const aiVector3D* b = &mesh->mBitangents[v];
                vertex.bitangent.x = float(b->x);
                vertex.bitangent.y = float(b->y);
                vertex.bitangent.z = float(b->z);
            }

            out->vertices.push_back(vertex);
        }

        for (size_t i = 0 ; i < mesh->mNumFaces ; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            assert(face.mNumIndices == 3);

            out->indices.push_back(face.mIndices[0]);
            out->indices.push_back(face.mIndices[1]);
            out->indices.push_back(face.mIndices[2]);
        }

        return out;
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    std::string loadTexture(const aiMaterial* const material,
                            const aiTextureType textureType)
    {
        if (material->GetTextureCount(textureType) <= 0)
            return "models/satellite/satellite_def_albedo.png";

        aiString path;
        if (material->GetTexture(textureType, 0, &path) != aiReturn_SUCCESS)
            return "models/satellite/satellite_def_albedo.png";

        return "models/satellite/" + std::string(path.C_Str());
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    std::shared_ptr<Material> importMaterial(const aiMaterial* const material)
    {
        std::shared_ptr<Material> out = std::make_shared<Material>();

        out->albedo = loadTexture(material, aiTextureType_DIFFUSE);
        //out->phong.diffuseSampler.setLinearizeGamma(true);
        //out->phong.specularSampler.setMap(loadTexture(material, aiTextureType_SPECULAR, dir));
        //out->phong.specularPowerSampler.setMap(loadTexture(material, aiTextureType_SHININESS, dir));
        //out->normalSampler.setMap(loadTexture(material, aiTextureType_HEIGHT, dir)); // Note "typo", it really needs to be aiTextureType_HEIGHT for OBJs

        std::cout << __FUNCTION__ << ", "
                 << material->GetTextureCount(aiTextureType_AMBIENT)   << ", "
                 << material->GetTextureCount(aiTextureType_DIFFUSE)   << ", "
                 << material->GetTextureCount(aiTextureType_SPECULAR)  << ", "
                 << material->GetTextureCount(aiTextureType_SHININESS) << ", "
                 << material->GetTextureCount(aiTextureType_NORMALS)   << ", "
                 << material->GetTextureCount(aiTextureType_HEIGHT)    << std::endl;;

        return out;
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    std::shared_ptr<Transform> importTransform(
        const aiString& name,
        const aiScene* const scene)
    {
        std::shared_ptr<Transform> t = std::make_shared<Transform>();
        t->scale = glm::vec3(1.0f);
        if (const aiNode* n = scene->mRootNode->FindNode(name))
        {
            glm::mat4 outMat(1.0f);
            {
                aiMatrix4x4 m = n->mTransformation;
                outMat = glm::mat4 {
                    m.a1, m.a2, m.a3, m.a4,
                    m.b1, m.b2, m.b3, m.b4,
                    m.c1, m.c2, m.c3, m.c4,
                    m.d1, m.d2, m.d3, m.d4
                };

                outMat = glm::transpose(outMat);
            }

            glm::quat rot;
            glm::vec3 pos, skew;
            glm::vec4 perspective;
            if (!glm::decompose(outMat, t->scale, rot, pos,
                                skew, perspective))
            {
                std::cerr << __FUNCTION__
                          << "Failed to decompose a transform matrix"
                          << std::endl;
            }

            t->position += pos;
            t->rotation *= glm::inverse(rot);//   * glm::angleAxis(glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //t->position *= 0.01;
            //t->scale    *= 0.01;
        }

        return t;
    }
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
ModelImporter::ModelImporter()
    : impl(std::make_shared<Impl>())
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
std::vector<ModelImporter::Model> ModelImporter::import(const std::string& filepath) const
{
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(
            filepath.c_str(),
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace);

    if (!scene)
        return {};

    std::cout << __FUNCTION__        << ", "
             << scene->mNumMeshes    << ", "
             << scene->mNumMaterials << ", "
             << scene->mNumTextures  << std::endl;

//    const QDir dir = QFileInfo(filepath).absoluteDir();

    std::vector<Model> out;
    for (unsigned int c = 0; c < scene->mRootNode->mNumChildren; ++c)
    {
        aiNode* child = scene->mRootNode->mChildren[c];
        if (!child)
            return {};

        for (unsigned int m = 0; m < child->mNumMeshes; ++m)
        {
            const aiMesh* const mesh =
                scene->mMeshes[child->mMeshes[m]];
            if (!mesh)
                return {};

//            qDebug() << __FUNCTION__
//                     << c
//                     << mesh->mNumVertices
//                     << mesh->mNumFaces
//                     << mesh->HasPositions()
//                     << mesh->HasNormals()
//                     << mesh->HasVertexColors(0)
//                     << mesh->HasTextureCoords(0)
//                     << mesh->HasTangentsAndBitangents();

            Model model;
            model.mesh = impl->importMesh(mesh);
            model.transform = impl->importTransform(child->mName, scene);
            if (scene->HasMaterials())
                model.material = impl->importMaterial(scene->mMaterials[mesh->mMaterialIndex]);
            out.push_back(model);
        }
    }
    return out;
}

glm::mat4 ModelImporter::Transform::matrix() const
{
    glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 r = glm::mat4_cast(rotation);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
    return t * r * s;
}

} // namespace sunne
} // namespace kuu
