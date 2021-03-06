/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLCompose vertex shader.
 * ---------------------------------------------------------------- */

#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
out struct VsOut
{
    vec2 texCoord;
} vsOut;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void main()
{
    vsOut.texCoord = texCoord;
    gl_Position = vec4(position, 0.0, 1.0);
}
