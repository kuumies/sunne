/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::OpenGLSatellite class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
class OpenGLSatellite
{
public:
    OpenGLSatellite();

    void loadResources();
    void draw();

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace sunne
} // namespace kuu
