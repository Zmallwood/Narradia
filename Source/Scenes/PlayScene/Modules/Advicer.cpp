#include "Advicer.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/PlayScene/Gui/MessageBox.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void Advicer::Update() {
        if (!has_given_foraging_advice_ && ready_for_next_advice_) {
            auto play_scene = PlayScene::Get();
            auto scene_gui = play_scene->GetSceneGui();
            auto gui_message_box = std::make_shared<MessageBox>(
                "Advice",
                "Its a good idea to start by foraging some edible items to regain your stamina.");
            scene_gui->AddGuiComponent(gui_message_box);
            has_given_foraging_advice_ = true;
            ready_for_next_advice_ = false;
        }
        else if (!has_given_create_stone_axe_advice_ && ready_for_next_advice_) {
            auto play_scene = PlayScene::Get();
            auto scene_gui = play_scene->GetSceneGui();
            auto gui_message_box = std::make_shared<MessageBox>(
                "Advice",
                "Now is a good time to create your first stone axe. Find a small stone and "
                "a branch and combine them.");
            scene_gui->AddGuiComponent(gui_message_box);
            has_given_create_stone_axe_advice_ = true;
            ready_for_next_advice_ = false;
        }
    }
}