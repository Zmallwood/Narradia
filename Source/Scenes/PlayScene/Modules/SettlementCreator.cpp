//////////////////////////////////////////////////////////////////////
#include "SettlementCreator.hpp"
#include "Engine/Core/TextOutBox.hpp"
#include "World/Companion.hpp"
#include "World/MapArea.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    SettlementCreator::CreateSettlement()
    /*//////////////////////////////////////*/
    {
        auto settlement_center = Player::Get().GetPosition().ToIntPoint();
        auto map_area = World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
        auto settlement_radius = 3;
        for (auto y = settlement_center.y - settlement_radius;
             y <= settlement_center.y + settlement_radius; y++)
        /*****************************************************/
        {
            for (auto x = settlement_center.x - settlement_radius;
                 x <= settlement_center.x + settlement_radius; x++)
            /*****************************************************/
            {
                auto dx = x - settlement_center.x;
                auto dy = y - settlement_center.y;
                if (dx * dx + dy * dy <= settlement_radius * settlement_radius)
                    Player::Get().ClaimTile({x, y});
            }
        }
        map_area->GetTile(settlement_center)->CreateAddObject("ObjectClaimFlag");
        auto companion_1_coordinate = settlement_center.Translate(-2, -2);
        auto companion_2_coordinate = settlement_center.Translate(2, -2);
        auto companion_3_coordinate = settlement_center.Translate(2, 2);
        auto companion_4_coordinate = settlement_center.Translate(-2, 2);
        auto companion1 = std::make_shared<Companion>(companion_1_coordinate);
        auto companion2 = std::make_shared<Companion>(companion_2_coordinate);
        auto companion3 = std::make_shared<Companion>(companion_3_coordinate);
        auto companion4 = std::make_shared<Companion>(companion_4_coordinate);
        companion2->ReverseDirection();
        companion4->ReverseDirection();
        map_area->GetTile(companion_1_coordinate)->SetCompanion(companion1);
        map_area->GetTile(companion_2_coordinate)->SetCompanion(companion2);
        map_area->GetTile(companion_3_coordinate)->SetCompanion(companion3);
        map_area->GetTile(companion_4_coordinate)->SetCompanion(companion4);
        map_area->AddCompanionMirror(companion1.get(), companion_1_coordinate);
        map_area->AddCompanionMirror(companion2.get(), companion_2_coordinate);
        map_area->AddCompanionMirror(companion3.get(), companion_3_coordinate);
        map_area->AddCompanionMirror(companion4.get(), companion_4_coordinate);
        TextOutBox::Get().Print("You have created a settlement.");
    }
}
//////////////////////////////////////////////////////////////////////