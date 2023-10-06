//////////////////////////////////////////////////////////////////////
#include "EditorScene.hpp"
#include "EditorSceneGui.hpp"
#include "Gui/EditorSceneGuiMenu.hpp"
#include "Modules/EditorSceneModulesCore.hpp"
#include "Modules/ToolUsing.hpp"
#include "Scenes/Shared/GuiObjectHovering.hpp"
#include "Scenes/Shared/KeyboardMove.hpp"
#include "Scenes/Shared/MouseRotation.hpp"
#include "Scenes/Shared/TileHovering.hpp"
#include "Scenes/Shared/WorldDrawModule/Camera.hpp"
#include "Scenes/Shared/WorldDrawModule/WorldDraw.hpp"
#include "World/MapArea.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
#include "Engine/Core/Audio.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    EditorScene::EditorScene()
    /*////////////////////*/
    {
        Log();
        SetSceneGui(std::make_shared<EditorSceneGui>());
        WorldDraw::Create();
        ObjectBehaviourList::Get().Clear();
        ObjectBehaviourList::Get().Initialize();
    }

    void EditorScene::Enter()
    /*////////////////////*/
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

    void EditorScene::UpdateDerived()
    /*////////////////////////////*/
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

    void EditorScene::RenderDerived()
    /*////////////////////////////*/
    {
        WorldDraw::Get().Render();
        TileHovering::Get().Render();
    }

    void EditorScene::RenderAfterGuiDerived()
    /*////////////////////////////////////*/
    {
        GuiObjectHovering::Get().Render();
    }
}
//////////////////////////////////////////////////////////////////////
