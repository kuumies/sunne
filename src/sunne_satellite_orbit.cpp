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
        static float totTime = 0.0f;
        totTime += elapsed;

        glm::quat rot = glm::angleAxis(glm::radians(0.16f), glm::vec3(-1.0f, 0.0f, 0.0f));
        satellite->rotation *= rot;

        if (totTime > 31000.0f)
        {
            static bool cut = false;
            if (!cut)
            {
                satellite->rotation = glm::quat(0.956647, glm::vec3(0.291218, 0.000000, 0.000000));
                cut = true;
            }
        }

        if (totTime >= 46000.0f)
        {
            static bool cut = false;
            if (!cut)
            {
                satellite->rotation = glm::quat(0.956647, glm::vec3(0.291218, 0.000000, 0.000000));
                cut = true;
            }
        }
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
