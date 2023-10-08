#include "ToolUsing.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "World/Actors/Mob.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void ToolUsing::Update() {
        MouseInput::Get()->GetLeftButton().AddFiredAction(
            "SelectObject",
            [&] {
                switch (current_tool_) {
                case Tools::AddObject: {
                    auto hovered_tile = TileHovering::Get()->hovered_tile_;
                    auto new_objects = std::make_shared<Object>(selected_type_);
                    if (selected_type_ != Hash("ObjectWoodWallN") &&
                        selected_type_ != Hash("ObjectWoodWallE") &&
                        selected_type_ != Hash("ObjectWoodWallS") &&
                        selected_type_ != Hash("ObjectWoodWallW")) {
                        new_objects->SetModelScaling(
                            0.7f + static_cast<float>(rand()) / RAND_MAX * 0.6f);
                        new_objects->SetModelRotation(
                            static_cast<float>(rand()) / RAND_MAX * 360.0f);
                    }
                    World::Get()
                        ->GetCurrentMapArea()
                        ->GetTile(hovered_tile)
                        ->AddObject(new_objects);
                    break;
                }
                case Tools::AddMob: {
                    auto hovered_tile = TileHovering::Get()->hovered_tile_;
                    auto new_mob =
                        std::make_shared<Mob>(selected_type_, hovered_tile.x, hovered_tile.y);
                    World::Get()->GetCurrentMapArea()->GetTile(hovered_tile)->SetMob(new_mob);
                    World::Get()->GetCurrentMapArea()->AddMobMirror(
                        World::Get()->GetCurrentMapArea()->GetTile(hovered_tile)->GetMob().get(),
                        hovered_tile);
                    break;
                }
                case Tools::SetGround: {
                    auto hovered_tile = TileHovering::Get()->hovered_tile_;
                    for (auto y = hovered_tile.y - (tool_radius_ - 1);
                         y <= hovered_tile.y + (tool_radius_ - 1); y++) {
                        for (auto x = hovered_tile.x - (tool_radius_ - 1);
                             x <= hovered_tile.x + (tool_radius_ - 1); x++) {
                            World::Get()->GetCurrentMapArea()->GetTile({x, y})->SetGroundType(
                                selected_type_);
                        }
                    }
                    World::Get()->ApplyDefaultColorVariations();
                    World::Get()->CalculateNormals(true);
                    break;
                }
                case Tools::SetRoof: {
                    auto hovered_tile = TileHovering::Get()->hovered_tile_;
                    World::Get()
                        ->GetCurrentMapArea()
                        ->GetTile(hovered_tile)
                        ->SetRoofType(selected_type_);
                    break;
                }
                case Tools::AlterElevation: {
                    auto hovered_tile = TileHovering::Get()->hovered_tile_;
                    for (auto y = hovered_tile.y - (tool_radius_ - 1);
                         y <= hovered_tile.y + (tool_radius_ - 1); y++) {
                        for (auto x = hovered_tile.x - (tool_radius_ - 1);
                             x <= hovered_tile.x + (tool_radius_ - 1); x++) {
                            auto dx = x - hovered_tile.x;
                            auto dy = y - hovered_tile.y;
                            if (dx * dx + dy * dy <= (tool_radius_ - 1) * (tool_radius_ - 1)) {
                                World::Get()->GetCurrentMapArea()->GetTile({x, y})->AlterElevation(
                                    elevation_change_);
                            }
                        }
                    }
                    World::Get()->ApplyDefaultColorVariations();
                    World::Get()->CalculateNormals(true);
                    break;
                }
                }
            },
            0);
    }

    void ToolUsing::IncreaseToolRadius() {
        tool_radius_++;
    }

    void ToolUsing::DecreaseToolRadius() {
        tool_radius_--;
    }

    void ToolUsing::IncreaseElevationChange() {
        elevation_change_++;
        if (elevation_change_ == 0)
            elevation_change_ = 1;
    }

    void ToolUsing::DecreaseElevationChange() {
        elevation_change_--;
        if (elevation_change_ == 0)
            elevation_change_ = -1;
    }

    void ToolUsing::ChangeTool(Tools newTool) {
        current_tool_ = newTool;
    }

    void ToolUsing::SelectType(int newSelectedObjectType) {
        selected_type_ = newSelectedObjectType;
    }
}