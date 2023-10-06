//////////////////////////////////////////////////////////////////////
#include "ObjectMoving.hpp"
#include "Engine/GuiCore.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "Scenes/PlayScene/Gui/InventoryGui.hpp"
#include "Scenes/PlayScene/Gui/OpenContainerGui.hpp"
#include "Scenes/PlayScene/ObjectHandling.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Scenes/Shared/TileHovering.hpp"
#include "World/Companion.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectMoving::ObjectMoving()
    /*////////////////////////*/
    {
        id_moving_object_image = Renderer2DImages::Get().NewImage();
    }

    void ObjectMoving::Update()
    /*///////////////////////*/
    {
        PickupObjectFromGuiIfIsTheCase();
        PickupObjectFromGroundIfIsTheCase();
        ReleaseObjectIfIsTheCase();
    }

    void ObjectMoving::PickupObjectFromGuiIfIsTheCase()
    /*///////////////////////////////////////////////*/
    {
        if (GuiWindowObjectSlot::hoveredObject)
        /*************************************/
        {
            MouseInput::Get().GetLeftButton().AddFiredAction(
                "ObjectMoveContainerObject", [=, this]()
                /**************************************/
                {
                    objectInAir = MoveObject(GuiWindowObjectSlot::hoveredObject);
                });
        }
    }

    void ObjectMoving::PickupObjectFromGroundIfIsTheCase()
    /*//////////////////////////////////////////////////*/
    {
        if (MapArea::IsInsideMap(TileHovering::Get().hoveredTile))
        /******************************************************/
        {
            auto player = Player::GetPointer();
            auto player_position = player->GetPosition().ToIntPoint();
            auto abs_dist_x = std::abs(TileHovering::Get().hoveredTile.x - player_position.x);
            auto abs_dist_y = std::abs(TileHovering::Get().hoveredTile.y - player_position.y);
            if (abs_dist_x <= 1 && abs_dist_y <= 1)
            /*************************************/
            {
                auto world = World::GetPointer();
                auto map_area = world->GetMapAreaAtZLevel(player->GetWorldAreaPos().z);
                auto tile = map_area->GetTile(TileHovering::Get().hoveredTile);
                if (tile->GetObjectsCount() > 0)
                /******************************/
                {
                    auto object_behaviour_list = ObjectBehaviourList::GetPointer();
                    if (object_behaviour_list->GetFlags(tile->GetObjectAt(0)->GetObjectType()) ==
                            0 ||
                        (object_behaviour_list->GetFlags(tile->GetObjectAt(0)->GetObjectType()) &
                         (int)ObjectBehaviourFlags::Unmovable) == 0)
                    /********************************************************************************/
                    {
                        MouseInput::Get().GetLeftButton().AddFiredAction(
                            "ObjectMovingReleaseObject",
                            [=, this]() { objectInAir = MoveObject(tile->GetObjectAt(0).get()); });
                    }
                }
            }
        }
    }

    void ObjectMoving::ReleaseObjectIfIsTheCase()
    /*/////////////////////////////////////////*/
    {
        MouseInput::Get().GetLeftButton().AddReleasedAction(
            "ObjectMovingReleaseObject",
            [&]
            /**************************/
            {
                if (objectInAir)
                /**************/
                {
                    if (GuiWindowObjectSlot::hoveredIndex != -1 &&
                        GuiWindowObjectSlot::activeWindow == InventoryGui::GetPointer().get())
                    /************************************************************************/
                    {
                        Player::Get().data.inventory.objects.insert(
                            {GuiWindowObjectSlot::hoveredIndex, MoveObject(objectInAir.get())});
                    }
                    else
                    /**/
                    {
                        for (auto &entry : PlayScene::Get().GetSceneGui()->GetGuiComponents())
                        /****************************************************************/
                        {
                            auto casted = std::dynamic_pointer_cast<OpenContainerGui>(entry);
                            if (casted)
                            /*********/
                            {
                                if (GuiWindowObjectSlot::hoveredIndex != -1 &&
                                    GuiWindowObjectSlot::activeWindow == entry.get())
                                /***************************************************/
                                {
                                    casted->object->AddContainedObject(
                                        GuiWindowObjectSlot::hoveredIndex,
                                        MoveObject(objectInAir.get()));
                                    return;
                                }
                            }
                        }
                        auto player_position = Player::Get().GetPosition().ToIntPoint();
                        auto map_area =
                            World::Get().GetMapAreaAtZLevel(Player::Get().GetWorldAreaPos().z);
                        auto tile = map_area->GetTile(TileHovering::Get().hoveredTile);
                        if (tile)
                        /*******/
                        {
                            if (tile->GetCompanion())
                            /***********************/
                            {
                                tile->GetCompanion()->GiveObject(MoveObject(objectInAir.get()));
                            }
                            else
                            /**/
                            {
                                tile->AddObject(MoveObject(objectInAir.get()));
                            }
                        }
                    }
                }
            },
            0);
    }

    void ObjectMoving::Render()
    /*///////////////////////*/
    {
        if (!objectInAir)
            return;
        auto mouse_position = GetMousePositionF();
        auto rectangle = RectangleF{
            mouse_position.x, mouse_position.y, k_object_image_size,
            ConvertWidthToHeight(k_object_image_size)};
        Renderer2DImages::Get().DrawImage(
            objectInAir->GetObjectType(), id_moving_object_image, rectangle);
    }
}
//////////////////////////////////////////////////////////////////////