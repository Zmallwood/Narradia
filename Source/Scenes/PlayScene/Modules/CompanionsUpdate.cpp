//////////////////////////////////////////////////////////////////////
#include "CompanionsUpdate.hpp"
#include "World/Companion.hpp"
#include "World/MapArea.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void CompanionsUpdate::Update()
    /*///////////////////////////*/
    {
        auto mapArea = World::Get().GetCurrentMapArea();
        for (auto it = mapArea->GetCompanionsMirror().cbegin();
             it != mapArea->GetCompanionsMirror().cend();)
        /*****************************************************/
        {
            auto companion = it->first;
            auto coordinate = it->second;
            if (companion->OutOfStamina())
            /****************************/
            {
                ++it;
                continue;
            }
            if (SDL_GetTicks() > companion->GetTicksLastMovement() + companion->GetMoveSpeed())
            /*********************************************************************************/
            {
                auto angle = companion->GetAngle();
                auto old_angle = angle;
                auto new_angle = angle + 2.0f * M_PI / (companion->GetRadius() * 20) *
                                             static_cast<int>(companion->GetDirection());
                if (new_angle >= 2 * M_PI || new_angle < 0)
                    companion->IncreaseRadius();
                new_angle = (((int)(new_angle * 180 / (M_PI)) + 360) % 360) * M_PI / 180.0f;
                auto new_dx = std::round(std::cos(new_angle)) * companion->GetRadius();
                auto new_dy = std::round(std::sin(new_angle)) * companion->GetRadius();
                auto new_x = companion->GetSpawnLocation().x + new_dx;
                auto new_y = companion->GetSpawnLocation().y + new_dy;
                float dx = new_x - coordinate.x;
                float dy = new_y - coordinate.y;
                float used_dx = std::min(1.0f, std::max(-1.0f, dx));
                float used_dy = std::min(1.0f, std::max(-1.0f, dy));
                float used_new_x = coordinate.x + used_dx;
                float used_new_y = coordinate.y + used_dy;
                auto new_x_i = static_cast<int>(used_new_x);
                auto new_y_i = static_cast<int>(used_new_y);
                companion->SetAngle(new_angle);
                if (new_x_i != coordinate.x || new_y_i != coordinate.y)
                /*****************************************************/
                {
                    companion->UpdateTicksLastMovement();
                    if (mapArea->GetTile({new_x_i, new_y_i})->GetCompanion() == nullptr)
                    /******************************************************************/
                    {
                        mapArea->GetTile({new_x_i, new_y_i})
                            ->SetCompanion(mapArea->GetTile(coordinate)->GetCompanion());
                        mapArea->GetTile(coordinate)->ClearCompanion();
                        companion->SetPreviousCoordinate(coordinate);
                        mapArea->RemoveCompanionMirror(it++);
                        mapArea->AddCompanionMirror(companion, {new_x_i, new_y_i});
                        if (false == Player::Get().HasClaimedTile({new_x_i, new_y_i}))
                        /**********************************************************/
                        {
                            Player::Get().ClaimTile({new_x_i, new_y_i});
                            companion->ConsumeStamina(1);
                            if (companion->OutOfStamina())
                            /****************************/
                            {
                                TextOutBox::Get().Print(
                                    "Companion: I am hungry, could you please give me some food?");
                            }
                        }
                        ++it;
                        continue;
                    }
                    else
                    /**/
                    {
                        companion->SetAngle(old_angle);
                        auto other_companion = mapArea->GetTile({new_x_i, new_y_i})->GetCompanion();
                        if (other_companion->GetAngle() > companion->GetAngle())
                            companion->ReverseDirection();
                        companion->DecreaseRadius();
                    }
                }
            }
            ++it;
        }
    }
}
//////////////////////////////////////////////////////////////////////
