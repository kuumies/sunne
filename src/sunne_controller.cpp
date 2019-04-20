/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::Controller class.
 * ---------------------------------------------------------------- */

#include "sunne_controller.h"
#include "renderer/opengl/sunne_opengl_renderer.h"
#include "renderer/sunne_renderer_scene.h"
#include "window/sunne_opengl_window.h"
#include "window/sunne_window_parameters.h"
#include "window/sunne_window_user_input.h"
#include "sunne_camera_orbit.h"
#include "sunne_satellite_orbit.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct Controller::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(Controller* self)
        : self(self)
        , closeApp(false)
        , resourceLoadStart(true)
        , resourceLoad(false)
    {}

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createScene()
    {
        scene = std::make_shared<RendererScene>();
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createWindow()
    {
        WindowParams params;
        params.opengl.major = 3;
        params.opengl.minor = 3;
        params.vSync        = true;
        params.title        = "Sunne";
        params.callback     = self;
        params.fullscreen   = false;
        params.size.x       = 960;
        params.size.y       = 540;

        window = std::make_shared<OpenGLWindow>(params);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createRenderer(const glm::ivec2& size)
    {
        renderer = std::make_shared<OpenGLRenderer>(size);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createCameraOrbit()
    {
        cameraOrbit = std::make_shared<CameraOrbit>(scene->camera);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void createSatelliteOrbit()
    {
        satelliteOrbit = std::make_shared<SatelliteOrbit>(scene->satellite);
    }


    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void resize(const glm::ivec2& size)
    {
        scene->camera->aspectRatio = size.x / float(size.y);
        renderer->resize(size);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void loadResources()
    {
        renderer->loadResources(scene);
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void render()
    {
        if (resourceLoad)
        {
            renderer->renderResourceLoadWait();
        }
        else
        {
            renderer->render(scene);
        }
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Controller* self;
    std::shared_ptr<CameraOrbit> cameraOrbit;
    std::shared_ptr<SatelliteOrbit> satelliteOrbit;
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<RendererScene> scene;
    bool closeApp;
    bool resourceLoadStart;
    bool resourceLoad;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
Controller::Controller()
    : impl(std::make_shared<Impl>(this))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::run()
{
    if (impl->window)
        return;
    impl->createScene();
    impl->createWindow();
    impl->createCameraOrbit();
    impl->createSatelliteOrbit();
    impl->window->run();
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::initialize(const glm::ivec2& size,
                            GLFWwindow* /*window*/)
{
    impl->scene->camera->aspectRatio = size.x / float(size.y);
    impl->createRenderer(size);
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::resize(const glm::ivec2& size)
{ impl->resize(size); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::update(double elapsed)
{
    if (impl->resourceLoad)
        return;

    impl->cameraOrbit->update(float(elapsed));
    impl->satelliteOrbit->update(float(elapsed));
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::render()
{ impl->render(); }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
bool Controller::closeApplication()
{ return impl->closeApp; }

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::setUserInput(const WindowUserInput& i)
{
    if (i.key.key == GLFW_KEY_ESCAPE)
        impl->closeApp = true;
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
bool Controller::startAsync()
{
    if (impl->resourceLoadStart)
    {
        impl->resourceLoadStart = false;
        impl->resourceLoad = true;
        return true;
    }
    else
    {
        return false;
    }
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void Controller::runAsync()
{
    impl->loadResources();
    impl->resourceLoad = false;
}

} // namespace sunne
} // namespace kuu
