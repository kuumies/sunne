/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLPlanet vertex shader.
 * ---------------------------------------------------------------- */

#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
    mat3 normal;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
uniform Matrices matrices;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
out struct VsOut
{
    vec2 texCoord;
    vec3 worldNormal;
    vec3 worldPos;
    vec3 cameraPos;
    mat3 tbn;

} vsOut;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void main()
{
    vec3 t = normalize(matrices.normal * tangent);
    vec3 b = normalize(matrices.normal * bitangent);
    vec3 n = normalize(matrices.normal * normal);

    // re-orthogonalize T with respect to N
    t = normalize(t - dot(t, n) * n);
    b = cross(n, t);

    vsOut.texCoord    = texCoord;
    vsOut.worldNormal = matrices.normal * normal;
    vsOut.worldPos    = vec3(matrices.model * vec4(position, 1.0));
    vsOut.cameraPos   = vec3(matrices.model * matrices.view * vec4(position, 1.0));
    vsOut.tbn         = mat3(t, b, n);

    gl_Position = matrices.projection *
                  matrices.view       *
                  matrices.model      *
                  vec4(position, 1.0);
}
