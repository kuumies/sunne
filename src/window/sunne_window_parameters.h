/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::WindowParams struct.
 * ---------------------------------------------------------------- */

#pragma once

/* ---------------------------------------------------------------- */

#include <glm/vec2.hpp>
#include <string>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- */

class WindowCallback;

/* ---------------------------------------------------------------- *
   Defines window parameters.
 * ---------------------------------------------------------------- */
struct WindowParams
{
    // Window title
    std::string title = "kuu::glfw::Window";

    // If true then the window is run fullscreen. The size
    // below defines the internal framebuffer size.
    bool fullscreen = false;

    // If true then user can resize the window.
    bool resizable = true;

    // Enable vertical synchronization.
    bool vSync = false;

    // Size of window.
    glm::ivec2 size = glm::ivec2(1024, 1024);

    // The callback is used for window to give in events to
    // other classes.
    WindowCallback* callback = nullptr;

    // Parameters for vulkan surface.
    struct Vulkan
    {} vulkan;

    // Parameters for OpenGL surface
    struct OpenGL
    {
        int major = 3;
        int minor = 3;
    } opengl;
};

} // namespace sunne
} // namespace kuu
