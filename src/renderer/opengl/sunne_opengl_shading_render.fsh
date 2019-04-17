/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLShadingRender fragment shader.
 * ---------------------------------------------------------------- */
 
#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
in struct VsOut
{
    vec2 texCoord;
    vec3 worldNormal;
    vec3 worldPos;
    mat3 tbn;

} vsOut;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
out vec4 outColor;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void main()
{
    vec3 n = normalize(vsOut.worldNormal);
    n = texture(normalMap, vsOut.texCoord).rgb;
    n = normalize(n * 2.0 - 1.0);
    n = vsOut.tbn * n;
    n = normalize(n);

    vec3 albedo = texture(albedoMap, vsOut.texCoord).rgb;

    vec3 lightDir = vec3(0, 1, 0);
    float nDotL = max(dot(n, lightDir), 0.0);
    outColor = vec4(vec3(nDotL) * albedo, 1.0);
    //outColor = vec4(n, 1.0);
}
