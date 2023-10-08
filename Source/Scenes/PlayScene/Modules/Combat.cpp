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
        if (SDL_GetTicks() < Player::Get()->data_.combat_.ticks_last_hit_given_other +
                                 Player::Get()->data_.combat_.attack_speed)
        /*********************************************************************/
        {
            return;
        }
        auto targeted_mob = MobTargeting::Get()->GetTargetedMob();
        if (nullptr != targeted_mob)
        /*************************/
        {
            auto player_world_area_pos = Player::Get()->GetWorldAreaPos();
            auto map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_pos.z);
            auto mob_coordinate = map_area->GetMobsMirror().at(targeted_mob);
            auto player_position = Player::Get()->GetPosition();
            auto dx = player_position.x - mob_coordinate.x;
            auto dy = player_position.y - mob_coordinate.y;
            auto dx_abs = std::abs(dx);
            auto dy_abs = std::abs(dy);
            if (dx_abs <= 0.8f && dy_abs <= 0.8f)
            /***************************/
            {
                auto damage = 1 + rand() % 3;
                switch (Player::Get()->current_player_build_)
                /********************************/
                {
                case PlayerBuilds::Sword:
                    /*******************/
                    if (Player::Get()->data_.status_.stamina <= 0)
                    /**************************************/
                    {
                        TextOutBox::Get()->Print("Your stamina is too low to hit with a sword.");
                    }
                    else
                    /**/
                    {
                        Audio::Get()->PlaySound("Punch",1);
                        targeted_mob->Hit(damage);
                        Player::Get()->data_.status_.stamina -= 1;
                        Player::Get()->data_.status_.rage += 1;
                        TextOutBox::Get()->Print(
                            "You hit mob with your sword for " + std::to_string(damage) +
                            " damage.");
                    }
                    break;
                case PlayerBuilds::Club:
                    /******************/
                    if (Player::Get()->data_.status_.rage <= 0)
                    /**************************************/
                    {
                        TextOutBox::Get()->Print("Your rage is too low to hit with a club.");
                    }
                    else
                    /**/
                    {
                        Audio::Get()->PlaySound("Punch",1);
                        targeted_mob->Hit(damage);
                        Player::Get()->data_.status_.rage -= 1;
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
                Player::Get()->data_.combat_.ticks_last_hit_given_other = SDL_GetTicks();
            }
        }
    }
}