/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   The definition of kuu::sunne::ModelImporter class.
 * ---------------------------------------------------------------- */
 
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class ModelImporter
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
    };

    struct Transform
    {
        glm::mat4 matrix() const;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    };

    struct Texture
    {
        int width;
        int height;
        int channels;
        unsigned char* pixels;
    };

    struct Material
    {
        std::string albedo;
    };

    struct Model
    {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Transform> transform;
        std::shared_ptr<Material> material;
    };

    ModelImporter();
    std::vector<Model> import(const std::string& filepath) const;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
