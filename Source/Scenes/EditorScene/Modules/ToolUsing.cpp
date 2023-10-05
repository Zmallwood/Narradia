//////////////////////////////////////////////////////////////////////
#include "ToolUsing.hpp"
#include "Scenes/Shared/TileHovering.hpp"
#include "World/MapArea.hpp"
#include "World/Mob.hpp"
#include "World/Object.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void ToolUsing::Update()
    /*//////////////////*/
    {
        MouseInput::Get().GetLeftButton().AddFiredAction(
            "SelectObject",
            [&]
            /*****************************/ {
                switch (currentTool)
                /******************/ {
                case Tools::AddObject:
                    /*****************/ {
                        auto hoveredTile = TileHovering::Get().hoveredTile;
                        auto newObject = std::make_shared<Object>(selectedType);
                        if (selectedType != Hash("ObjectWoodWallN") &&
                            selectedType != Hash("ObjectWoodWallE") &&
                            selectedType != Hash("ObjectWoodWallS") &&
                            selectedType != Hash("ObjectWoodWallW"))
                        /********************************************/ {
                            newObject->SetModelScaling(
                                0.7f + static_cast<float>(rand()) / RAND_MAX * 0.6f);
                            newObject->SetModelRotation(
                                static_cast<float>(rand()) / RAND_MAX * 360.0f);
                        }
                        World::Get()
                            .GetCurrentMapArea()
                            ->GetTile(hoveredTile)
                            ->AddObject(newObject);
                        break;
                    }
                case Tools::AddMob:
                    /**************/ {
                        auto hoveredTile = TileHovering::Get().hoveredTile;
                        auto newMob =
                            std::make_shared<Mob>(selectedType, hoveredTile.x, hoveredTile.y);
                        World::Get().GetCurrentMapArea()->GetTile(hoveredTile)->SetMob(newMob);
                        World::Get().GetCurrentMapArea()->AddMobMirror(
                            World::Get().GetCurrentMapArea()->GetTile(hoveredTile)->GetMob().get(),
                            hoveredTile);
                        break;
                    }
                case Tools::SetGround:
                    /*****************/ {
                        auto hoveredTile = TileHovering::Get().hoveredTile;
                        for (auto y = hoveredTile.y - (toolRadius - 1);
                             y <= hoveredTile.y + (toolRadius - 1); y++)
                        /**********************************************/ {
                            for (auto x = hoveredTile.x - (toolRadius - 1);
                                 x <= hoveredTile.x + (toolRadius - 1); x++)
                            /**********************************************/ {
                                World::Get().GetCurrentMapArea()->GetTile({x, y})->SetGroundType(
                                    selectedType);
                            }
                        }
                        World::Get().ApplyDefaultColorVariations();
                        World::Get().CalculateNormals(true);
                        break;
                    }
                case Tools::SetRoof:
                    /***************/ {
                        auto hoveredTile = TileHovering::Get().hoveredTile;
                        World::Get()
                            .GetCurrentMapArea()
                            ->GetTile(hoveredTile)
                            ->SetRoofType(selectedType);
                        break;
                    }
                case Tools::AlterElevation:
                    /**********************/ {
                        auto hoveredTile = TileHovering::Get().hoveredTile;
                        for (auto y = hoveredTile.y - (toolRadius - 1);
                             y <= hoveredTile.y + (toolRadius - 1); y++)
                        /**********************************************/
                        {
                            for (auto x = hoveredTile.x - (toolRadius - 1);
                                 x <= hoveredTile.x + (toolRadius - 1); x++)
                            /**********************************************/
                            {
                                auto dx = x - hoveredTile.x;
                                auto dy = y - hoveredTile.y;
                                if (dx * dx + dy * dy <= (toolRadius - 1) * (toolRadius - 1))
                                /***********************************************************/ {
                                    World::Get()
                                        .GetCurrentMapArea()
                                        ->GetTile({x, y})
                                        ->AlterElevation(elevationChange);
                                }
                            }
                        }
                        World::Get().ApplyDefaultColorVariations();
                        World::Get().CalculateNormals(true);
                        break;
                    }
                }
            },
            0);
    }

    void ToolUsing::IncreaseToolRadius()
    /*//////////////////////////////*/
    {
        toolRadius++;
    }

    void ToolUsing::DecreaseToolRadius()
    /*//////////////////////////////*/
    {
        toolRadius--;
    }

    void ToolUsing::IncreaseElevationChange()
    /*///////////////////////////////////*/
    {
        elevationChange++;
        if (elevationChange == 0)
            elevationChange = 1;
    }

    void ToolUsing::DecreaseElevationChange()
    /*///////////////////////////////////*/
    {
        elevationChange--;
        if (elevationChange == 0)
            elevationChange = -1;
    }

    void ToolUsing::ChangeTool(Tools newTool)
    /*///////////////////////////////////*/
    {
        currentTool = newTool;
    }

    void ToolUsing::SelectType(int newSelectedObjectType)
    /*///////////////////////////////////////////////*/
    {
        selectedType = newSelectedObjectType;
    }
}
//////////////////////////////////////////////////////////////////////