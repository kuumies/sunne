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

        const float rotPerSecond = 10.0f;
        const float seconds = elapsed / 1000.0f;
        const float rotInc = seconds * rotPerSecond;
        std::cout << "sat rot inc: " << rotInc << std::endl;
        //glm::quat rot = glm::angleAxis(glm::radians(rotInc), glm::vec3(-1.0f, 0.0f, 0.0f));
        glm::quat rot = glm::angleAxis(glm::radians(0.16f), glm::vec3(-1.0f, 0.0f, 0.0f));
        satellite->rotation *= rot;
        //std::cout << "<< totTime << std::endl;

        if (totTime > 30000.0f)
        {
            static bool cut = false;
            if (!cut)
            {
                satellite->rotation = glm::quat(0.956647, glm::vec3(0.291218, 0.000000, 0.000000));
                cut = true;
            }
        }

        if (totTime > 50000.0f)
        {
            // cut to somewhere else
        }

        // quat(-0.436747, {0.899577, 0.000000, 0.000000})
        // quat(0.956647, {0.291218, 0.000000, 0.000000})
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
