/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::WindowCallback class.
 * ---------------------------------------------------------------- */

#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <string>

/* ---------------------------------------------------------------- */

struct GLFWwindow;

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- */

struct WindowUserInput;

/* ---------------------------------------------------------------- *
   Defines window callback.
 * ---------------------------------------------------------------- */
class WindowCallback
{
public:
    // Destructor
    virtual ~WindowCallback();

    // Initialize is called before rendering loop.
    virtual void initialize(const glm::ivec2& size,
                            GLFWwindow* window) = 0;

    // Resize is called when the window framebuffer
    // size changes.
    virtual void resize(const glm::ivec2& size) = 0;

    // Update is called per frame, elapsed is in milliseconds since
    // last frame.
    virtual void update(double elapsed) = 0;
    // Render is called per frame.
    virtual void render() = 0;

    // Return true to close application.
    virtual bool closeApplication() = 0;
    // Return true to start a asynchronous job, e.g.
    // resource load. This is polled every frame.
    virtual bool startAsync();

    // A function that is run asynchronously.
    virtual void runAsync();

    // Sets an user input.
    virtual void setUserInput(const WindowUserInput& i) = 0;
};

} // namespace sunne
} // namespace kuu
