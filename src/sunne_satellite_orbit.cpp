/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::SatelliteOrbit class.
 * ---------------------------------------------------------------- */

#include "sunne_satellite_orbit.h"
#include "window/sunne_window_user_input.h"
#include <iostream>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct SatelliteOrbit::Impl
{
    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    Impl(SatelliteOrbit* self, std::shared_ptr<RendererScene::Satellite> satellite)
        : self(self)
        , satellite(satellite)
    {}

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void update(float elapsed)
    {
        const float rotPerSecond = 2.0f;
        const float seconds = elapsed / 1000.0f;
        const float rotInc = seconds * rotPerSecond;
        glm::quat rot = glm::angleAxis(glm::radians(rotInc), glm::vec3(-1.0f, 0.0f, 0.0f));
        satellite->rotation *= rot;
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    SatelliteOrbit* self;
    std::shared_ptr<RendererScene::Satellite> satellite;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
SatelliteOrbit::SatelliteOrbit(std::shared_ptr<RendererScene::Satellite> satellite)
    : impl(std::make_shared<Impl>(this, satellite))
{}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void SatelliteOrbit::update(float elapsed)
{ impl->update(elapsed); }

} // namespace sunne
} // namespace kuu
