#include "Combat.hpp"
#include "Engine/Core/TextOutBox.hpp"
#include "MobTargeting.hpp"
#include "World/Actors/Mob.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/World.hpp"
#include "Engine/Core/Audio.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    Combat::Update()
    /*////////////*/
    {
        if (SDL_GetTicks() < Player::Get()->data.combat.ticksLastHitGivenOther +
                                 Player::Get()->data.combat.attackSpeed)
        /*********************************************************************/
        {
            return;
        }
        auto targetedMob = MobTargeting::Get()->GetTargetedMob();
        if (nullptr != targetedMob)
        /*************************/
        {
            auto playerWorldAreaPos = Player::Get()->GetWorldAreaPos();
            auto mapArea = World::Get()->GetMapAreaAtZLevel(playerWorldAreaPos.z);
            auto mobCoordinate = mapArea->GetMobsMirror().at(targetedMob);
            auto playerPos = Player::Get()->GetPosition();
            auto dx = playerPos.x - mobCoordinate.x;
            auto dy = playerPos.y - mobCoordinate.y;
            auto dxAbs = std::abs(dx);
            auto dyAbs = std::abs(dy);
            if (dxAbs <= 0.8f && dyAbs <= 0.8f)
            /***************************/
            {
                auto damage = 1 + rand() % 3;
                switch (Player::Get()->playerBuild)
                /********************************/
                {
                case PlayerBuilds::Sword:
                    /*******************/
                    if (Player::Get()->data.stats.stam <= 0)
                    /**************************************/
                    {
                        TextOutBox::Get()->Print("Your stamina is too low to hit with a sword.");
                    }
                    else
                    /**/
                    {
                        Audio::Get()->PlaySound("Punch",1);
                        targetedMob->Hit(damage);
                        Player::Get()->data.stats.stam -= 1;
                        Player::Get()->data.stats.rage += 1;
                        TextOutBox::Get()->Print(
                            "You hit mob with your sword for " + std::to_string(damage) +
                            " damage.");
                    }
                    break;
                case PlayerBuilds::Club:
                    /******************/
                    if (Player::Get()->data.stats.rage <= 0)
                    /**************************************/
                    {
                        TextOutBox::Get()->Print("Your rage is too low to hit with a club.");
                    }
                    else
                    /**/
                    {
                        Audio::Get()->PlaySound("Punch",1);
                        targetedMob->Hit(damage);
                        Player::Get()->data.stats.rage -= 1;
                        TextOutBox::Get()->Print(
                            "You hit mob with your club for " + std::to_string(damage) +
                            " damage.");
                    }
                    break;
                case PlayerBuilds::None:
                    /******************/
                    TextOutBox::Get()->Print(
                        "You try to attack mob but have not equipped any weapon.");
                    break;
                }
                Player::Get()->data.combat.ticksLastHitGivenOther = SDL_GetTicks();
            }
        }
    }
}