//////////////////////////////////////////////////////////////////////
#include "Advicer.hpp"
#include "Scenes/PlayScene/Gui/PlaySceneGuiCore.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void Advicer::Update()
    /*//////////////////*/
    {
        if (!has_given_foraging_advice && ready_for_next_advie)
        /*****************************************************/
        {
            auto main_scene = PlayScene::GetPointer();
            auto scene_gui = main_scene->GetSceneGui();
            auto gui_message_box = std::make_shared<MessageBox>(
                "Advice",
                "Its a good idea to start by foraging some edible items to regain your stamina.");
            scene_gui->AddGuiComponent(gui_message_box);
            has_given_foraging_advice = true;
            ready_for_next_advie = false;
        }
        else if (!has_given_create_stone_axe_advice && ready_for_next_advie)
        /********************************************************************/
        {
            auto main_scene = PlayScene::GetPointer();
            auto scene_gui = main_scene->GetSceneGui();
            auto gui_message_box = std::make_shared<MessageBox>(
                "Advice",
                "Now is a good time to create your first stone axe. Find a small stone and "
                "a branch and combine them.");
            scene_gui->AddGuiComponent(gui_message_box);
            has_given_create_stone_axe_advice = true;
            ready_for_next_advie = false;
        }
    }
}
//////////////////////////////////////////////////////////////////////