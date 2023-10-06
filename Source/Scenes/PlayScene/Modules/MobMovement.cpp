//////////////////////////////////////////////////////////////////////
#include "MobMovement.hpp"
#include "World/MapArea.hpp"
#include "World/Mob.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    MobMovement::Update()
    /*//////////////////////*/
    {
        Log();
        auto player_world_area_position = Player::Get().GetWorldAreaPos();
        if (nullptr == World::GetPointer())
            return;
        Log();
        auto map_area = World::Get().GetMapAreaAtZLevel(player_world_area_position.z);
        if (nullptr == map_area)
            return;
        for (auto it = map_area->GetMobsMirror().cbegin(); it != map_area->GetMobsMirror().cend();)
        /*****************************************************************************************/
        {
            auto mob = it->first;
            auto coordinate = it->second;
            Log();
            if (SDL_GetTicks() > mob->GetTicksLastMovement() + mob->GetMoveSpeed())
            /*********************************************************************/
            {
                if (mob->IsAggroingPlayer()) {
                    mob->SetDestination(Player::Get().GetPosition().ToIntPoint());
                }
                else if (mob->HasNoDestination())
                /*********************************/
                {
                    int new_dest_x;
                    int new_dest_y;
                    if (mob->GetMobType() == Hash("MobTypeBird1"))
                    /********************************************/
                    {
                        new_dest_x = coordinate.x + rand() % 8 - rand() % 8;
                        new_dest_y = coordinate.y + rand() % 8 - rand() % 8;
                    }
                    else
                    /**/
                    {
                        auto spawnX = mob->GetSpawnX();
                        auto spawnY = mob->GetSpawnY();
                        new_dest_x = spawnX + rand() % 3 - rand() % 3;
                        new_dest_y = spawnY + rand() % 3 - rand() % 3;
                    }
                    new_dest_x = std::min(MapArea::GetMapSize().width - 1, std::max(0, new_dest_x));
                    new_dest_y =
                        std::min(MapArea::GetMapSize().height - 1, std::max(0, new_dest_y));
                    mob->SetDestination({new_dest_x, new_dest_y});
                }
                auto delta_x = mob->GetDestination().x - coordinate.x;
                auto delta_y = mob->GetDestination().y - coordinate.y;
                auto abs_delta_x = std::abs(delta_x);
                auto abs_delta_y = std::abs(delta_y);
                auto norm_x = 0;
                auto norm_y = 0;
                if (delta_x)
                    norm_x = delta_x / abs_delta_x;
                if (delta_y)
                    norm_y = delta_y / abs_delta_y;
                auto dx = norm_x;
                auto dy = norm_y;
                auto new_x = coordinate.x + dx;
                auto new_y = coordinate.y + dy;
                if (Point2{new_x, new_y} == mob->GetDestination())
                    mob->ClearDestination();
                if (new_x != coordinate.x || new_y != coordinate.y)
                /*************************************************/
                {
                    if (MapArea::IsInsideMap({new_x, new_y}))
                    /***************************************/
                    {
                        if (mob->GetMobType() == Hash("MobTypeBird1") ||
                            map_area->GetTile({new_x, new_y})->GetGroundType() !=
                                Hash("GroundWater"))
                        /****************************************************************************/
                        {
                            if (map_area->GetTile({new_x, new_y})->GetMob() == nullptr)
                            /*********************************************************/
                            {
                                Log();
                                map_area->GetTile({new_x, new_y})
                                    ->SetMob(map_area->GetTile(coordinate)->GetMob());
                                Log();
                                map_area->GetTile(coordinate)->ClearMob();
                                Log();
                                map_area->RemoveMobMirror(it++);
                                Log();
                                map_area->AddMobMirror(mob, {new_x, new_y});
                                Log();
                                mob->UpdateTicksLastMovement();
                                mob->SetPreviousCoordinate(coordinate);
                                continue;
                            }
                            else
                            /**/
                            {
                                mob->ClearDestination();
                            }
                        }
                        else
                        /**/
                        {
                            mob->ClearDestination();
                        }
                    }
                }
            }
            ++it;
        }
    }
}
//////////////////////////////////////////////////////////////////////
