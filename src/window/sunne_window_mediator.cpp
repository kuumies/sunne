/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::WindowMediator class.
 * ---------------------------------------------------------------- */

#include "sunne_window_mediator.h"
#include <iostream>
#include "sunne_window_callback.h"
#include "sunne_window_user_input.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
WindowMediator& WindowMediator::getInstance()
{
    static WindowMediator instance;
    return instance;
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void WindowMediator::errorCallback(
    int error,
    const char* description)
{
    std::cerr << __FUNCTION__ << ": "
              << error << " " << description
              << std::endl;
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void WindowMediator::framebufferSizeCallback(
    GLFWwindow* /*window*/,
    int width,
    int height)
{
    WindowCallback* callback = getInstance().callback;
    if (callback)
        callback->resize(glm::ivec2(width, height));
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void WindowMediator::keyCallback(
    GLFWwindow* /*window*/,
    int key,
    int scancode,
    int action,
    int mods)
{
    WindowCallback* callback = getInstance().callback;
    if (callback)
    {
        WindowUserInput i(WindowUserInput::Type::Key);
        i.key.key = key;
        i.key.status = action;
        i.key.modifiers = mods;
        callback->setUserInput(i);
    }
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void WindowMediator::mouseButtonCallback(
    GLFWwindow* window,
    int button,
    int action,
    int mods)
{
    WindowCallback* callback = getInstance().callback;
    if (callback)
    {
        glm::dvec2 p;
        glfwGetCursorPos(window, &p.x, &p.y);

        WindowUserInput i(WindowUserInput::Type::Mouse);
        i.mouse.button = button;
        i.mouse.status = action;
        i.mouse.modifiers = mods;
        i.mouse.pos = p;
        callback->setUserInput(i);
    }
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void WindowMediator::cursorPosCallback(
    GLFWwindow* /*window*/,
    double x,
    double y)
{
    WindowCallback* callback = getInstance().callback;
    if (callback)
    {
        glm::dvec2 p(x, y);

        WindowUserInput i(WindowUserInput::Type::Cursor);
        i.cursor.pos = p;
        callback->setUserInput(i);
    }
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void WindowMediator::scrollCallback(
    GLFWwindow* /*window*/,
    double x,
    double y)
{
    WindowCallback* callback = getInstance().callback;
    if (callback)
    {
        WindowUserInput i(WindowUserInput::Type::Wheel);
        i.wheel.pos = glm::vec2(x, y);
        callback->setUserInput(i);
    }
}

} // namespace sunne
} // namespace kuu
