//////////////////////////////////////////////////////////////////////
#include "Combat.hpp"
#include "Engine/Core/TextOutBox.hpp"
#include "MobTargeting.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    Combat::Update()
    /*******************/
    {
        if (SDL_GetTicks() < Player::Get().data.combat.ticksLastHitGivenOther +
                                 Player::Get().data.combat.attackSpeed)
        /**********************************************************************/
        {
            return;
        }
        auto targetedMob = MobTargeting::Get().GetTargetedMob();
        if (nullptr != targetedMob)
        /*************************/
        {
            auto playerWorldAreaPos = Player::Get().GetWorldAreaPos();
            auto mapArea = World::Get().GetMapAreaAtZLevel(playerWorldAreaPos.z);
            auto mobCoordinate = mapArea->GetMobsMirror().at(targetedMob);
            auto playerPos = Player::Get().GetPosition();
            auto dx = playerPos.x - mobCoordinate.x;
            auto dy = playerPos.y - mobCoordinate.y;
            auto dxAbs = std::abs(dx);
            auto dyAbs = std::abs(dy);
            if (dxAbs <= 0.5f && dyAbs <= 0.5f)
            /***************************/
            {
                auto damage = 1 + rand() % 3;
                switch (Player::Get().playerBuild)
                /********************************/
                {
                case PlayerBuilds::Sword:
                    /*******************/
                    TextOutBox::Get().Print(
                        "You hit mob with your sword for " + std::to_string(damage) + " damage.");
                    break;
                case PlayerBuilds::Club:
                    /******************/
                    TextOutBox::Get().Print(
                        "You hit mob with your club for " + std::to_string(damage) + " damage.");
                    break;
                case PlayerBuilds::None:
                    /******************/
                    TextOutBox::Get().Print(
                        "You try to attack mob but have not equipped any weapon.");
                    break;
                }
                Player::Get().data.combat.ticksLastHitGivenOther = SDL_GetTicks();
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////