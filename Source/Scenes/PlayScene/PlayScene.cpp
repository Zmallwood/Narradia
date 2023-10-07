//////////////////////////////////////////////////////////////////////
#include "PlayScene.hpp"
#include "Engine/Core/Audio.hpp"
#include "Gui/PlaySceneGuiMenu.hpp"
#include "Modules/ActionRepeat.hpp"
#include "Modules/ActiveGameRound.hpp"
#include "Modules/Advicer.hpp"
#include "Modules/Combat.hpp"
#include "Modules/CompanionsUpdate.hpp"
#include "Modules/DestinationMove.hpp"
#include "Modules/InteractionMenu.hpp"
#include "Modules/MobMovement.hpp"
#include "Modules/MobTargeting.hpp"
#include "Modules/ObjectMoving.hpp"
#include "Modules/ObjectTransformation.hpp"
#include "Modules/PlaySceneHotkeys.hpp"
#include "Modules/SettlementCreator.hpp"
#include "Modules/StartConditions.hpp"
#include "PlaySceneGui.hpp"
#include "Scenes/SharedSceneModules/GuiObjectHovering.hpp"
#include "Scenes/SharedSceneModules/KeyboardMove.hpp"
#include "Scenes/SharedSceneModules/MouseRotation.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/WorldDraw.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    PlayScene::PlayScene()
    /*////////////////*/
    {
        Log();
        SetSceneGui(std::make_shared<PlaySceneGui>());
        Log();
        Player::Create();
        Log();
        WorldDraw::Create();
        Log();
        TileHovering::Create();
        Log();
        InteractionMenu::Create();
        Log();
        ObjectMoving::Create();
    }

    void
    PlayScene::Enter()
    /*//////////////////*/
    {
        PlaySceneGuiMenu::Get().visible = false;
        ActiveGameRound::Create();
        StartConditions().CreateStartConditions();
        SettlementCreator().CreateSettlement();
        WorldDraw::Get().EnablePlayMode();
        DestinationMove::Get().ResetDestination();
        Audio::Get().Unmute();
    }

    void
    PlayScene::UpdateDerived()
    /*//////////////////////////*/
    {
        Log();
        ActiveGameRound::Get().Update();
        if (ActiveGameRound::Get().GameIsCompleted())
            return;
        Log();
        PlaySceneHotkeys().Update();
        if (true == PlaySceneGuiMenu::Get().visible)
            return;
        Log();
        ActionRepeat::Get().Update();
        Log();
        ObjectTransformation::Get().Update();
        Log();
        DestinationMove::Get().Update();
        Log();
        InteractionMenu::Get().Update();
        Log();
        MouseRotation::Get().Update();
        Log();
        Player::Get().Update();
        Log();
        ObjectMoving::Get().Update();
        Log();
        KeyboardMove().Update();
        Log();
        TileHovering::Get().Update();
        Log();
        WorldDraw::Get().Update();
        Log();
        CompanionsUpdate().Update();
        Log();
        MobMovement::Get().Update();
        Log();
        Advicer::Get().Update();
        Log();
        MobTargeting::Get().Update();
        Log();
        Combat::Get().Update();
    }

    void
    PlayScene::RenderDerived()
    /*//////////////////////////*/
    {
        Log();
        WorldDraw::Get().Render();
        if (true == PlaySceneGuiMenu::Get().visible)
        /******************************************/
        {
            Log();
            TileHovering::Get().Render();
        }
        ActiveGameRound::Get().Render();
    }

    void
    PlayScene::RenderAfterGuiDerived()
    /*//////////////////////////////////*/
    {
        InteractionMenu::Get().Render();
        GuiObjectHovering::Get().Render();
        ObjectMoving::Get().Render();
    }
}
//////////////////////////////////////////////////////////////////////