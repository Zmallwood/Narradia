//////////////////////////////////////////////////////////////////////
#include "DestinationMove.hpp"
#include "Scenes/Shared/WorldDrawModule/Camera.hpp"
#include "InteractionMenu.hpp"
#include "ObjectMoving.hpp"
#include "Scenes/Shared/TileHovering.hpp"
#include "World/Player.hpp"
#include "World/World.hpp"
#include "World/MapArea.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void DestinationMove::Update()
    /*//////////////////////////*/ {
        if (Camera::Get().cameraDistance == 2.0f) return;
        auto player_world_area_position = Player::Get().GetWorldAreaPos();
        auto map_area = World::Get().GetMapAreaAtZLevel(player_world_area_position.z);
        if (MapArea::IsInsideMap(TileHovering::Get().hoveredTile))
        /******************************************************/ {
            MouseInput::Get().GetLeftButton().AddFiredAction(
                "DestMovementSetDest",
                [&]
                /*********************/ {
                    if (SDL_GetTicks() > InteractionMenu::Get().GetTicksClosed() + 400)
                    /************************************************************/ {
                        destination = TileHovering::Get().hoveredTile;
                    }
                },
                0, 0);
        }
        if ((destination.x != -1 && destination.y != -1))
        /***********************************************/ {
            auto player_position = Player::Get().GetPosition();
            auto dx = destination.x + 0.5f - player_position.x;
            auto dy = destination.y + 0.5f - player_position.y;
            if (dx != 0 || dy != 0)
            /*********************/ {
                auto dist = std::sqrt(dx * dx + dy * dy);
                if (dist >= 0.2f)
                /***************/ {
                    auto angle = static_cast<float>(std::atan2(dx, dy) * 180.f / M_PI) + 180.f;
                    Player::Get().SetFacingAngle(angle);
                    Player::Get().MoveForward();
                    Player::Get().data.movement.isMoving = true;
                } else
                /****/ {
                    Player::Get().data.deltaT = 0;
                    destination = {-1, -1};
                    Player::Get().data.movement.isMoving = false;
                }
            }
        }
    } // Function

    void DestinationMove::ResetDestination()
    /*////////////////////////////////////*/ { destination = {-1, -1}; } // Function

    const Point2 &DestinationMove::GetDestination()
    /*///////////////////////////////////////////*/ { return destination; } // Function
}
//////////////////////////////////////////////////////////////////////