/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::WindowMediator class.
 * ---------------------------------------------------------------- */

#pragma once

/* ---------------------------------------------------------------- */

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

class WindowCallback;

/* ---------------------------------------------------------------- *
   A mediator between Window and GLFWwindow classes. This class
   handles the GLFW callbacks and propagates them into Window
   callback.
 * ---------------------------------------------------------------- */
class WindowMediator
{
public:
    // Returns singleton instance of the window mediator.
    static WindowMediator& getInstance();

    // GLFW error
    static void errorCallback(
        int error,
        const char* description);

    // GLFW window framebuffer size has changed.
    static void framebufferSizeCallback(
        GLFWwindow* /*window*/,
        int width,
        int height);

    // Key callback
    static void keyCallback(
        GLFWwindow* /*window*/,
        int key,
        int scancode,
        int action,
        int mods);

    // Mouse callback
    static void mouseButtonCallback(
        GLFWwindow* window,
        int button,
        int action,
        int mods);

    // Cursor position callback
    static void cursorPosCallback(
        GLFWwindow* window,
        double x,
        double y);

    // Mouse wheel
    static void scrollCallback(
        GLFWwindow* /*window*/,
        double /*x*/,
        double /*y*/);

private:
    WindowMediator(){}
    WindowMediator(WindowMediator const&);
    void operator=(WindowMediator const&);

public:
    WindowCallback* callback = nullptr;
};

} // namespace sunne
} // namespace kuu
