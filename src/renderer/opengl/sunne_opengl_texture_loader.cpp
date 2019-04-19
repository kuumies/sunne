/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::opengl_texture_loader namespace.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_texture_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace kuu
{
namespace sunne
{
namespace opengl_texture_loader
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
GLuint load(const std::string& path, int req_comp, bool sRgb)
{
    int imgW, imgH, imgC;
    stbi_uc* pixels = stbi_load(
        path.c_str(),
        &imgW,
        &imgH,
        &imgC,
        req_comp);

    if (!pixels)
        throw std::runtime_error(
            std::string(__FUNCTION__) +
                ": failed to load image " +
                path);

    GLenum format, internalFormat;
    switch(imgC)
    {
        case 1: internalFormat = format = GL_RED;  break;
        case 2: internalFormat = format = GL_RG;   break;
        case 3: format = GL_RGB;  internalFormat = sRgb ? GL_SRGB       : GL_RGB;  break;
        case 4: format = GL_RGBA; internalFormat = sRgb ? GL_SRGB_ALPHA : GL_RGBA; break;
        default:
            throw std::runtime_error(
                std::string(__FUNCTION__) +
                    ": failed to load texture " +
                    path +
                    " as it has invalid channel count of " +
                    std::to_string(imgC));
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D,  tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GLint(internalFormat),
                 imgW, imgH, 0,
                 format, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    if (GL_TEXTURE_MAX_ANISOTROPY_EXT)
    {
        GLfloat anisotropy = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  0);
    return tex;
}

} // namespace opengl_texture_loader
} // namespace sunne
} // namespace kuu
