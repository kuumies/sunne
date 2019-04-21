/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLPlanet fragment shader.
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
    vec2 texCoordCloud;
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
    n = texture(normalMap, vsOut.texCoord).rgb;
    n = normalize(n * 2.0 - 1.0);
    n = vsOut.tbn * n;
    n = normalize(n);

    vec3 v = normalize(-vsOut.cameraPos);
    vec3 l = normalize(vec3(1, 1, 1));
    vec3 r = reflect(-l, n);
    vec2 tc = vsOut.texCoord;

    float nDotL = max(dot(l, n), 0.0);
    float vDotR = max(dot(v, r), 0.0);

    vec3 albedo = vec3(0.0);
    if (nDotL > 0)
    {
        albedo = texture(albedoMap, tc).rgb;
        vec4 clouds = texture(cloudMap,  vsOut.texCoordCloud);
        albedo = mix(albedo, clouds.rgb, clouds.a) * nDotL;
    }
    else
        albedo = texture(nightMap, tc).rgb;

    vec3 diffuse  = albedo /** nDotL*/;
    vec3 specular = vec3(texture(specularMap, tc).r) * pow(vDotR, 128.0);

    outColor = vec4(diffuse, 1.0);
}
