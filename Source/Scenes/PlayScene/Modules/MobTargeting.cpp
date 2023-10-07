//////////////////////////////////////////////////////////////////////
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
        Mob *targetedMob = nullptr;
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
        auto hoveredTile = TileHovering::Get()->hoveredTile;
        if (MapArea::IsInsideMap(hoveredTile))
        /************************************/
        {
            auto playerWorldAreaPos = Player::Get()->GetWorldAreaPos();
            auto mapArea = World::Get()->GetMapAreaAtZLevel(playerWorldAreaPos.z);
            auto tile = mapArea->GetTile(hoveredTile);
            if (nullptr != tile->GetMob())
            /****************************/
            {
                auto action = [=] { p->targetedMob = tile->GetMob().get(); };
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
                        p->targetedMob = nullptr;
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
        return p->targetedMob;
    }
}
//////////////////////////////////////////////////////////////////////
