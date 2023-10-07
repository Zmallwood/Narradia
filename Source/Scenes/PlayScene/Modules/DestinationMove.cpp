#include "DestinationMove.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "InteractionMenu.hpp"
#include "ObjectMoving.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    DestinationMove::Update()
    /*/////////////////////*/
    {
        if (Camera::Get()->cameraDistance == 2.0f)
            return;
        auto player_world_area_position = Player::Get()->GetWorldAreaPos();
        auto map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_position.z);
        if (MapArea::IsInsideMap(TileHovering::Get()->hoveredTile))
        /******************************************************/
        {
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "DestMovementSetDest",
                [&]
                /*********************/
                {
                    if (SDL_GetTicks() > InteractionMenu::Get()->GetTicksClosed() + 400)
                    /************************************************************/
                    {
                        destination_ = TileHovering::Get()->hoveredTile;
                    }
                },
                0, 0);
        }
        if ((destination_.x != -1 && destination_.y != -1))
        /***********************************************/
        {
            auto player_position = Player::Get()->GetPosition();
            auto dx = destination_.x + 0.5f - player_position.x;
            auto dy = destination_.y + 0.5f - player_position.y;
            if (dx != 0 || dy != 0)
            /*********************/
            {
                auto dist = std::sqrt(dx * dx + dy * dy);
                if (dist >= 0.2f)
                /***************/
                {
                    auto angle = static_cast<float>(std::atan2(dx, dy) * 180.f / M_PI) + 180.f;
                    Player::Get()->SetFacingAngle(angle);
                    Player::Get()->MoveForward();
                    Player::Get()->data.movement.isMoving = true;
                }
                else
                /**/
                {
                    Player::Get()->data.deltaT = 0;
                    destination_ = {-1, -1};
                    Player::Get()->data.movement.isMoving = false;
                }
            }
        }
    }

    void
    DestinationMove::ResetDestination()
    /*///////////////////////////////*/
    {
        destination_ = {-1, -1};
    }

    const Point2 &
    DestinationMove::GetDestination()
    /*/////////////////////////////*/
    {
        return destination_;
    }
}