#include "ObjectMoving.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/PlayScene/Gui/InventoryGui.hpp"
#include "Scenes/PlayScene/Gui/OpenContainerGui.hpp"
#include "Scenes/PlayScene/ObjectHandling.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "World/Actors/Companion.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectMoving::ObjectMoving()
    /*////////////////////////*/
    {
        rendid_moving_object_image_ = Renderer2DImages::Get()->NewImage();
    }

    void
    ObjectMoving::Update()
    /*//////////////////*/
    {
        PickupObjectFromGuiIfIsTheCase();
        PickupObjectFromGroundIfIsTheCase();
        ReleaseObjectIfIsTheCase();
    }

    void
    ObjectMoving::PickupObjectFromGuiIfIsTheCase()
    /*//////////////////////////////////////////*/
    {
        if (GuiWindowObjectSlot::hovered_object_)
        /*************************************/
        {
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "ObjectMoveContainerObject", [=, this]()
                /**************************************/
                { object_in_air_ = MoveObject(GuiWindowObjectSlot::hovered_object_); });
        }
    }

    void
    ObjectMoving::PickupObjectFromGroundIfIsTheCase()
    /*/////////////////////////////////////////////*/
    {
        if (MapArea::IsInsideMap(TileHovering::Get()->hovered_tile_))
        /********************************************************/
        {
            auto player = Player::Get();
            auto player_position = player->GetPosition().ToIntPoint();
            auto abs_dist_x = std::abs(TileHovering::Get()->hovered_tile_.x - player_position.x);
            auto abs_dist_y = std::abs(TileHovering::Get()->hovered_tile_.y - player_position.y);
            if (abs_dist_x <= 1 && abs_dist_y <= 1)
            /*************************************/
            {
                auto world = World::Get();
                auto map_area = world->GetMapAreaAtZLevel(player->GetWorldAreaPos().z);
                auto tile = map_area->GetTile(TileHovering::Get()->hovered_tile_);
                if (tile->GetObjectsCount() > 0)
                /******************************/
                {
                    auto object_behaviour_list = ObjectBehaviourList::Get();
                    if (object_behaviour_list->GetFlags(tile->GetObjectAt(0)->GetObjectType()) ==
                            0 ||
                        (object_behaviour_list->GetFlags(tile->GetObjectAt(0)->GetObjectType()) &
                         (int)ObjectBehaviourFlags::Unmovable) == 0)
                    /********************************************************************************/
                    {
                        MouseInput::Get()->GetLeftButton().AddFiredAction(
                            "ObjectMovingReleaseObject",
                            [=, this]() { object_in_air_ = MoveObject(tile->GetObjectAt(0).get()); });
                    }
                }
            }
        }
    }

    void
    ObjectMoving::ReleaseObjectIfIsTheCase()
    /*////////////////////////////////////*/
    {
        MouseInput::Get()->GetLeftButton().AddReleasedAction(
            "ObjectMovingReleaseObject",
            [&]
            /**************************/
            {
                if (object_in_air_)
                /**************/
                {
                    if (GuiWindowObjectSlot::hovered_index_ != -1 &&
                        GuiWindowObjectSlot::active_window_ == InventoryGui::Get().get())
                    /************************************************************************/
                    {
                        Player::Get()->data_.inventory_.objects_.insert(
                            {GuiWindowObjectSlot::hovered_index_, MoveObject(object_in_air_.get())});
                    }
                    else
                    /**/
                    {
                        for (auto &entry : PlayScene::Get()->GetSceneGui()->GetGuiComponents())
                        /****************************************************************/
                        {
                            auto casted = std::dynamic_pointer_cast<OpenContainerGui>(entry);
                            if (casted)
                            /*********/
                            {
                                if (GuiWindowObjectSlot::hovered_index_ != -1 &&
                                    GuiWindowObjectSlot::active_window_ == entry.get())
                                /***************************************************/
                                {
                                    casted->object->AddContainedObject(
                                        GuiWindowObjectSlot::hovered_index_,
                                        MoveObject(object_in_air_.get()));
                                    return;
                                }
                            }
                        }
                        auto player_position = Player::Get()->GetPosition().ToIntPoint();
                        auto map_area =
                            World::Get()->GetMapAreaAtZLevel(Player::Get()->GetWorldAreaPos().z);
                        auto tile = map_area->GetTile(TileHovering::Get()->hovered_tile_);
                        if (tile)
                        /*******/
                        {
                            if (tile->GetCompanion())
                            /***********************/
                            {
                                tile->GetCompanion()->GiveObject(MoveObject(object_in_air_.get()));
                            }
                            else
                            /**/
                            {
                                tile->AddObject(MoveObject(object_in_air_.get()));
                            }
                        }
                    }
                }
            },
            0);
    }

    void
    ObjectMoving::Render()
    /*//////////////////*/
    {
        if (!object_in_air_)
            return;
        auto mouse_position = GetMousePositionF();
        auto rectangle = RectangleF{
            mouse_position.x, mouse_position.y, kObjectImageSize,
            ConvertWidthToHeight(kObjectImageSize)};
        Renderer2DImages::Get()->DrawImage(
            object_in_air_->GetObjectType(), rendid_moving_object_image_, rectangle);
    }
}