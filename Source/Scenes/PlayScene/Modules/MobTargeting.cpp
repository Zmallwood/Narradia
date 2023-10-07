#include "MobTargeting.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Scenes/SharedSceneModules/MouseRotation.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MobTargeting::Pimpl
    /*/////////////////////*/
    {
      public:
        Mob *targeted_mob_ = nullptr;
    };

    MobTargeting::MobTargeting()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    MobTargeting::Update()
    /*//////////////////*/
    {
        auto hovered_tile = TileHovering::Get()->hoveredTile;
        if (MapArea::IsInsideMap(hovered_tile))
        /************************************/
        {
            auto player_world_area_pos = Player::Get()->GetWorldAreaPos();
            auto map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_pos.z);
            auto tile = map_area->GetTile(hovered_tile);
            if (nullptr != tile->GetMob())
            /****************************/
            {
                auto action = [=] { p->targeted_mob_ = tile->GetMob().get(); };
                MouseInput::Get()->GetRightButton().AddFiredAction(
                    "TargetHoveredMob", action, 100000);
            }
            else
            /**/
            {
                auto action = [=]
                /***************/
                {
                    if (MouseInput::Get()->GetRightButton().ClickDurationPickResult() <
                        MouseButton::GetDefaultClickSpeed())
                    /****************************************************************/
                    {
                        p->targeted_mob_ = nullptr;
                    }
                };
                MouseInput::Get()->GetRightButton().AddReleasedAction(
                    "TargetHoveredMob", action, 0, 0, true);
            }
        }
    }

    Mob *
    MobTargeting::GetTargetedMob()
    /*//////////////////////////*/
    {
        return p->targeted_mob_;
    }
}