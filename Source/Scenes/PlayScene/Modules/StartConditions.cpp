//////////////////////////////////////////////////////////////////////
#include "StartConditions.hpp"
#include "World/MapArea.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void StartConditions::CreateStartConditions()
    /*/////////////////////////////////////////*/
    {
        auto &inventory = Player::Get().data.inventory;
        inventory.Add("ObjectKindling");
        inventory.Add("ObjectFishingNet");
        Player::Get().ClearClaimedTiles();
        Player::Get().data.movement.position = {
            MapArea::GetMapSize().width / 2.0f, MapArea::GetMapSize().height / 2.0f};
        ObjectBehaviourList::Get().Clear();
        ObjectBehaviourList::Get().Initialize();
    }
}
//////////////////////////////////////////////////////////////////////