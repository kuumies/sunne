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
    //vec3 color = texture(shadingTexMap, vsOut.texCoord).rgb +
    //             texture(atmosphereTexMap, vsOut.texCoord).rgb +
    //             texture(starTexMap, vsOut.texCoord).rgb;

    vec3 color = texture(shadingTexMap, vsOut.texCoord).rgb;
    //color = 1.0 - exp(-exposure * color);
    color = pow(color.rgb, vec3(1.0/2.2));
    outColor = vec4(color, 1.0);
}
