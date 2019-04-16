/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::Controller class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#include "window/sunne_window_callback.h"

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class Controller : public WindowCallback
{
public:
    Controller();
    void run();

    // WindowCallback overrides
    void initialize(const glm::ivec2& size, GLFWwindow* window) override;
    void resize(const glm::ivec2& size) override;
    void update(double elapsed) override;
    void render() override;
    bool closeApplication() override;
    void setUserInput(const WindowUserInput& i) override;

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
