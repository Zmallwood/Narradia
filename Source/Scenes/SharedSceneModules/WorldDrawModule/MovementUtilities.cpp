//////////////////////////////////////////////////////////////////////
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
        auto mapArea = World::Get()->GetCurrentMapArea();
        auto coord = mapArea->GetMobsMirror().at(mob);
        auto dx = coord.x - mob->GetPreviousCoordinate().x;
        auto dy = coord.y - mob->GetPreviousCoordinate().y;
        auto movementProgress =
            static_cast<float>(SDL_GetTicks() - mob->GetTicksLastMovement()) / mob->GetMoveSpeed();
        movementProgress = std::min(1.0f, movementProgress);
        auto minorX = -(1.0f - movementProgress) * dx;
        auto minorY = -(1.0f - movementProgress) * dy;
        return {minorX, minorY};
    }

    Point2F
    GetMinorMovementOffsetForCompanion(Companion *companion)
    /*////////////////////////////////////////////////////*/
    {
        auto mapArea = World::Get()->GetCurrentMapArea();
        auto coord = mapArea->GetCompanionsMirror().at(companion);
        auto dx = coord.x - companion->GetPreviousCoordinate().x;
        auto dy = coord.y - companion->GetPreviousCoordinate().y;
        auto movementProgress =
            static_cast<float>(SDL_GetTicks() - companion->GetTicksLastMovement()) /
            companion->GetMoveSpeed();
        movementProgress = std::min(1.0f, movementProgress);
        auto minorX = -(1.0f - movementProgress) * dx;
        auto minorY = -(1.0f - movementProgress) * dy;
        return {minorX, minorY};
    }
}