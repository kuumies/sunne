/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::Window class.
 * ---------------------------------------------------------------- */

#pragma once

/* ---------------------------------------------------------------- */

struct GLFWwindow;

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- */

class WindowCallback;
struct WindowParams;

/* ---------------------------------------------------------------- *
   A window base class.
 * ---------------------------------------------------------------- */
class Window
{
public:
    // Constructs the window. This will throw std::runtime_error
    // if the window creation fails.
    Window(const WindowParams& params);
    // Destroys the window.
    virtual ~Window();

    // Starts the process loop for rendering and polling the user
    // actions that runs until window is closed.
    virtual void run();

protected:
    WindowCallback* callback_ = nullptr;
    GLFWwindow* window_ = nullptr;
};

} // namespace sunne
} // namespace kuu
