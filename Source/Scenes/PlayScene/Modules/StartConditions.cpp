#include "StartConditions.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void StartConditions::CreateStartConditions() {
        auto &inventory_ = Player::Get()->data_.inventory_;
        inventory_.Add("ObjectKindling");
        inventory_.Add("ObjectFishingNet");
        Player::Get()->ClearClaimedTiles();
        Player::Get()->data_.movement_.position = {
            MapArea::GetMapSize().width / 2.0f, MapArea::GetMapSize().height / 2.0f};
        ObjectBehaviourList::Get()->Clear();
        ObjectBehaviourList::Get()->Initialize();
    }
}