#include "SubDrawerSky.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    SubDrawerSky::DrawSky()
    /*/////////////////*/
    {
        const auto player_world_area_pos = Player::Get()->GetWorldAreaPos();
        if (player_world_area_pos.z >= 0)
            glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}