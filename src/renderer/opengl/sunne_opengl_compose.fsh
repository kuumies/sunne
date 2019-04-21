/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLCompose fragment shader.
 * ---------------------------------------------------------------- */

#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
uniform sampler2D shadingTexMap;
uniform sampler2D atmosphereTexMap;
uniform sampler2D starTexMap;
uniform sampler2D planetTexMap;
uniform float exposure;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
in struct VsOut
{
    vec2 texCoord;
} vsOut;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
out vec4 outColor;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void main()
{
    vec4 scene      = texture(shadingTexMap,    vsOut.texCoord);
    vec3 planet     = texture(planetTexMap,     vsOut.texCoord).rgb;
    vec3 atmosphere = texture(atmosphereTexMap, vsOut.texCoord).rgb;
    vec3 earth = mix(planet, atmosphere, 0.6);

    vec3 color = vec3(0.0);
    if (scene.a > 0)
        color = scene.rgb;
    else
         color = earth;

    //color = texture(atmosphereTexMap, vsOut.texCoord).rgb;
    color = 1.0 - exp(-exposure * color);
    color = pow(color.rgb, vec3(1.0/2.2));
    outColor = vec4(color, 1.0);
}
