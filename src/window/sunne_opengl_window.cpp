/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::OpenGLWindow class.
 * ---------------------------------------------------------------- */

#include "sunne_opengl_window.h"
#include <future>
#include <iostream>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sunne_window_callback.h"
#include "sunne_window_mediator.h"
#include "sunne_window_parameters.h"
#include "sunne_window_user_input.h"

namespace kuu
{
namespace sunne
{
namespace
{

/* ---------------------------------------------------------------- *
   Run a callback job asynchronously.
 * ---------------------------------------------------------------- */
void runAsyncJob(GLFWwindow* window, WindowCallback* callback)
{
    glfwMakeContextCurrent(window);
    callback->runAsync();
    //glFlush(); // Needs to be called, otherwise no textures
    //glfwMakeContextCurrent(nullptr);
}

} // anonymous namespace

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct OpenGLWindow::Data
{
    // GLFW windows.
    GLFWwindow* windowTheading = nullptr;

    // Async job
    std::future<void> asyncJob;

    // Prev time.
    double prevTime = 0.0;

    // Window title.
    std::string windowTitle;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
OpenGLWindow::OpenGLWindow(const WindowParams& params)
    : Window(params)
    , d(std::make_shared<Data>())
{
    d->windowTitle = params.title;

    glfwSetErrorCallback(&WindowMediator::errorCallback);
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.opengl.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.opengl.minor);
    if (params.opengl.major >= 3)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        if (params.opengl.major >= 3)
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif



    glfwWindowHint(GLFW_RESIZABLE, params.resizable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

    window_ = glfwCreateWindow(
        params.fullscreen ? videoMode->width  : params.size.x,
        params.fullscreen ? videoMode->height : params.size.y,
        (params.title + std::string(" - OpenGL")).c_str(),
        params.fullscreen
            ? monitor
            : nullptr,
        nullptr);

    if (!window_)
    {
        throw std::runtime_error(
            std::string(__FUNCTION__)
                + ": failed to create GLFW window");
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    d->windowTheading =
        glfwCreateWindow(1, 1, "Threading Window",
                         nullptr, window_);

    callback_ = params.callback;
    WindowMediator::getInstance().callback = callback_;

    glfwSetFramebufferSizeCallback(
        window_,
        &WindowMediator::framebufferSizeCallback);

    glfwSetKeyCallback(
        window_,
        &WindowMediator::keyCallback);

    glfwSetMouseButtonCallback(
        window_,
        &WindowMediator::mouseButtonCallback);

    glfwSetCursorPosCallback(
        window_,
        &WindowMediator::cursorPosCallback);

    glfwSetScrollCallback(
        window_,
        &WindowMediator::scrollCallback);

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(params.vSync ? 1 : 0);

    if (gladLoadGL() == 0)
        throw std::runtime_error(
            std::string(__FUNCTION__) +
            ": failed to load OpenGL");
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void OpenGLWindow::run()
{
    // Make the OpenGL current on the window with this thread.
    //glfwMakeContextCurrent(window_);
    //if (gladLoadGL() == 0)
    //    throw std::runtime_error(
    //        std::string(__FUNCTION__) +
    //        ": failed to load OpenGL");

    // Reset timer.
    glfwSetTime(0.0);

    // Initialize
    if (callback_)
    {
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        callback_->initialize(glm::ivec2(width, height), window_);
    }

    // Start loop.
    int frameCounter = 0;
    double elapsedCounter = 0.0;
    while (!glfwWindowShouldClose(window_))
    {
        if (callback_)
        {
            if (callback_->closeApplication())
                glfwSetWindowShouldClose(window_, true);

            if (callback_->startAsync())
            {
                if (!d->asyncJob.valid())
                    d->asyncJob = std::async(
                        std::launch::async,
                        runAsyncJob,
                        d->windowTheading,
                        callback_);
            }

            if (d->asyncJob.valid() &&
                d->asyncJob.wait_for(std::chrono::milliseconds(1))
                    == std::future_status::ready)
            {
                d->asyncJob.get();
                glfwMakeContextCurrent(window_);
            }

            const double time = glfwGetTime();
            const double elapsed = time - d->prevTime;
            d->prevTime = time;
            callback_->update(elapsed * 1000.0);
            callback_->render();

            frameCounter++;
            elapsedCounter += (elapsed * 1000.0);
            if (elapsedCounter >= 1000.0)
            {
                std::string title = d->windowTitle;
                title += " -  OpenGL";
                title += " - ";
                title += std::to_string(frameCounter);
                title += " FPS";

                glfwSetWindowTitle(window_, title.c_str());
                frameCounter = 0;
                elapsedCounter = 0.0;
            }
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    if (d->asyncJob.valid())
        d->asyncJob.wait();

    // Destroys the window.
    glfwDestroyWindow(window_);
}

} // namespace sunne
} // namespace kuu
