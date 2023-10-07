//////////////////////////////////////////////////////////////////////
#include "EditorScene.hpp"
#include "EditorSceneGui.hpp"
#include "Engine/Core/Audio.hpp"
#include "Gui/EditorSceneGuiMenu.hpp"
#include "Modules/EditorSceneHotkeys.hpp"
#include "Modules/ToolUsing.hpp"
#include "Scenes/SharedSceneModules/GuiObjectHovering.hpp"
#include "Scenes/SharedSceneModules/KeyboardMove.hpp"
#include "Scenes/SharedSceneModules/MouseRotation.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/WorldDraw.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    EditorScene::EditorScene()
    /*//////////////////////*/
    {
        Log();
        SetSceneGui(std::make_shared<EditorSceneGui>());
        WorldDraw::Create();
        ObjectBehaviourList::Get().Clear();
        ObjectBehaviourList::Get().Initialize();
    }

    void
    EditorScene::Enter()
    /*////////////////*/
    {
        EditorSceneGuiMenu::Get().visible = false;
        if (nullptr == World::Get().GetMapAreaAtZLevel(0))
        /**********************************************/
        {
            auto newMapArea = std::make_shared<MapArea>();
            newMapArea->Create();
            World::Get().AddMapAreaAtZLevel(0, newMapArea);
            World::Get().ResetColorVariations();
            World::Get().GenerateRandomColorVariations();
            World::Get().ApplyDefaultColorVariations();
            World::Get().CalculateNormals();
        }
        Player::Get().data.movement.position = {
            MapArea::GetMapSize().width / 2.0f, MapArea::GetMapSize().height / 2.0f};
        WorldDraw::Get().EnableMapEditorMode();
        Audio::Get().Mute();
        Camera::Get().cameraDistance = 800.0f;
    }

    void
    EditorScene::UpdateDerived()
    /*////////////////////////*/
    {
        TileHovering::Get().Update();
        EditorSceneHotkeys().Update();
        Log();
        Player::Get().Update();
        KeyboardMove().Update();
        MouseRotation::Get().Update();
        WorldDraw::Get().Update();
        ToolUsing::Get().Update();
    }

    void
    EditorScene::RenderDerived()
    /*////////////////////////*/
    {
        WorldDraw::Get().Render();
        TileHovering::Get().Render();
    }

    void
    EditorScene::RenderAfterGuiDerived()
    /*////////////////////////////////*/
    {
        GuiObjectHovering::Get().Render();
    }
}
//////////////////////////////////////////////////////////////////////
