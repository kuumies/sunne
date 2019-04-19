/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLLoading fragment shader.
 * ---------------------------------------------------------------- */
 
#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
uniform sampler2D tex;

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
    outColor = texture(tex, vsOut.texCoord);
}