/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Implementation of kuu::sunne::SatelliteOrbit class.
 * ---------------------------------------------------------------- */

#include "sunne_satellite_orbit.h"
#include "window/sunne_window_user_input.h"
#include <glm/gtx/string_cast.hpp>
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
    {
        satellite->rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    void update(float elapsed)
    {
        totTime += elapsed;

        const float step = 0.16f; // note: v-sync is enabled
        glm::quat rot = glm::angleAxis(glm::radians(step), glm::vec3(-1.0f, 0.0f, 0.0f));
        satellite->rotation *= rot;

        const float cutTimeA = 31000.0f;
        const float cutTimeB = 46000.0f;
        const glm::quat cutRot = glm::quat(0.956647f, glm::vec3(0.3f, 0.0f, 0.0f));

        if (totTime >= cutTimeA && !cutSetA)
        {
            satellite->rotation = cutRot;
            cutSetA = true;
        }

        if (totTime >= cutTimeB && !cutSetB)
        {
            satellite->rotation = cutRot;
            cutSetB = true;
        }
    }

    /* ------------------------------------------------------------ *
     * ------------------------------------------------------------ */
    SatelliteOrbit* self;
    std::shared_ptr<RendererScene::Satellite> satellite;
    float totTime = 0.0f;
    bool cutSetA = false;
    bool cutSetB = false;
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
