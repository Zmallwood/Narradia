//////////////////////////////////////////////////////////////////////
#include "PlayScene.hpp"
#include "Modules/Advicer.hpp"
#include "Modules/CompanionsUpdate.hpp"
#include "Modules/DestinationMove.hpp"
#include "Scenes/Shared/GuiObjectHovering.hpp"
#include "Modules/InteractionMenu.hpp"
#include "Scenes/Shared/KeyboardMove.hpp"
#include "Modules/MobMovement.hpp"
#include "Scenes/Shared/MouseRotation.hpp"
#include "Modules/ObjectMoving.hpp"
#include "Modules/ObjectTransformation.hpp"
#include "PlaySceneGui.hpp"
#include "Gui/PlaySceneGuiCore.hpp"
#include "Gui/PlaySceneGuiMenu.hpp"
#include "Modules/PlaySceneModulesCore.hpp"
#include "Modules/SettlementCreator.hpp"
#include "Modules/StartConditions.hpp"
#include "Scenes/Shared/TileHovering.hpp"
#include "Scenes/Shared/WorldDrawModule/WorldDraw.hpp"
#include "Universe/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    PlayScene::PlayScene()
    /*////////////////*/ {
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
    } // Function

    void PlayScene::Enter()
    /*//////////////////*/ {
        PlaySceneGuiMenu::Get().visible = false;
        ActiveGameRound::Create();
        StartConditions().CreateStartConditions();
        SettlementCreator().CreateSettlement();
        WorldDraw::Get().EnablePlayMode();
        DestinationMove::Get().ResetDestination();
        Audio::Get().Unmute();
    } // Function

    void PlayScene::UpdateDerived()
    /*//////////////////////////*/ {
        Log();
        ActiveGameRound::Get().Update();
        if (ActiveGameRound::Get().GameIsCompleted()) return;
        Log();
        PlaySceneHotkeys().Update();
        if (true == PlaySceneGuiMenu::Get().visible) return;
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
    } // Function

    void PlayScene::RenderDerived()
    /*//////////////////////////*/ {
        Log();
        WorldDraw::Get().Render();
        if (true == PlaySceneGuiMenu::Get().visible)
        /*****************************************************/ {
            Log();
            TileHovering::Get().Render();
        }
        ActiveGameRound::Get().Render();
    } // Function

    void PlayScene::RenderAfterGuiDerived()
    /*//////////////////////////////////*/ {
        InteractionMenu::Get().Render();
        GuiObjectHovering::Get().Render();
        ObjectMoving::Get().Render();
    } // Function
}
//////////////////////////////////////////////////////////////////////