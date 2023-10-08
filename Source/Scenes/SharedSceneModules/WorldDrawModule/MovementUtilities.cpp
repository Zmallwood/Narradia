#include "MovementUtilities.hpp"
#include "Camera.hpp"
#include "World/Actors/Companion.hpp"
#include "World/MapArea.hpp"
#include "World/Actors/Mob.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point2F
    GetMinorMovementOffsetForMob(Mob *mob)
    /*//////////////////////////////////*/
    {
        auto map_area = World::Get()->GetCurrentMapArea();
        auto coord = map_area->GetMobsMirror().at(mob);
        auto dx = coord.x - mob->GetPreviousCoordinate().x;
        auto dy = coord.y - mob->GetPreviousCoordinate().y;
        auto movement_progress =
            static_cast<float>(SDL_GetTicks() - mob->GetTicksLastMovement()) / mob->GetMoveSpeed();
        movement_progress = std::min(1.0f, movement_progress);
        auto minor_x = -(1.0f - movement_progress) * dx;
        auto minor_y = -(1.0f - movement_progress) * dy;
        return {minor_x, minor_y};
    }

    Point2F
    GetMinorMovementOffsetForCompanion(Companion *companion)
    /*////////////////////////////////////////////////////*/
    {
        auto map_area = World::Get()->GetCurrentMapArea();
        auto coord = map_area->GetCompanionsMirror().at(companion);
        auto dx = coord.x - companion->GetPreviousCoordinate().x;
        auto dy = coord.y - companion->GetPreviousCoordinate().y;
        auto movement_progress =
            static_cast<float>(SDL_GetTicks() - companion->GetTicksLastMovement()) /
            companion->GetMoveSpeed();
        movement_progress = std::min(1.0f, movement_progress);
        auto minor_x = -(1.0f - movement_progress) * dx;
        auto minor_y = -(1.0f - movement_progress) * dy;
        return {minor_x, minor_y};
    }
}