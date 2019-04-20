/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLSatellite fragment shader.
 * ---------------------------------------------------------------- */
 
#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D cloudMap;
uniform sampler2D nightMap;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
in struct VsOut
{
    vec2 texCoord;
    vec3 worldNormal;
    vec3 worldPos;
    vec3 cameraPos;
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
//    n = texture(normalMap, vsOut.texCoord).rgb;
//    n = normalize(n * 2.0 - 1.0);
//    n = vsOut.tbn * n;
//    n = normalize(n);

    vec3 v = normalize(-vsOut.cameraPos);
    vec3 l = normalize(vec3(1, 1, 1));
    vec3 r = reflect(-l, n);
    vec2 tc = vsOut.texCoord;

    float nDotL = max(dot(l, n), 0.0);
    float vDotR = max(dot(v, r), 0.0);

    vec3 albedo = texture(albedoMap, tc).rgb;
    vec3 diffuse  = albedo * nDotL;
    vec3 specular = vec3(1.0) * pow(vDotR, 128.0);

    outColor = vec4(diffuse + specular, 1.0);
}
