/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLWindow class.
 * ---------------------------------------------------------------- */

#pragma once

/* ---------------------------------------------------------------- */

#include <memory>
#include "sunne_window.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- */

struct WindowParams;

/* ---------------------------------------------------------------- *
   A window to display double-buffered OpenGL 2.1 or 3.3 rendered
   content and to capture user keyboard and mouse actions.

   Window supports asynchronous calls with the shared OpenGL
   context. This is achieved with Callback.
 * ---------------------------------------------------------------- */
class OpenGLWindow : public Window
{
public:
    // Constructs the window. This will throw std::runtime_error
    // if the window creation fails.
    OpenGLWindow(const WindowParams& params);

    // Starts the process loop for rendering and polling the user
    // actions that runs until window is closed.
    void run();

private:
    struct Data;
    std::shared_ptr<Data> d;
};

} // namespace sunne
} // namespace kuu
