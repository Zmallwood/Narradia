#include "SplitStackGui.hpp"
#include "Engine/GuiCore/GuiTextBox.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/PlayScene/Modules/ActionRepeat.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "World/Actors/Player.hpp"
#include "World/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    SplitStackGui::SplitStackGui(Object *object_)
        : GuiWindow("Split", {0.15f, 0.2f, 0.17f, 0.15f}),
          object(object_) {
        Show();
        auto quantity = object_->GetQuantity();
        auto text = std::to_string(quantity / 2);
        auto left_text_box_change_event = std::make_shared<std::function<void()>>();
        auto right_text_box_change_event = std::make_shared<std::function<void()>>();
        auto left_text_box = std::make_shared<GuiTextBox>(
            RectangleF{0.01f, 0.05f, 0.05f, 0.03f}, this, text, left_text_box_change_event);
        auto right_text_box = std::make_shared<GuiTextBox>(
            RectangleF{0.11f, 0.05f, 0.05f, 0.03f}, this, text, right_text_box_change_event);
        *left_text_box_change_event = [=] {
            if (left_text_box->GetText() == "") {
                right_text_box->SetText(std::to_string(quantity));
            }
            else {
                auto new_value = std::stoi(left_text_box->GetText().data());
                auto other_value = quantity - new_value;
                right_text_box->SetText(std::to_string(other_value));
            }
        };
        *right_text_box_change_event = [=] {
            if (right_text_box->GetText() == "") {
                left_text_box->SetText(std::to_string(quantity));
            }
            else {
                auto new_value = std::stoi(right_text_box->GetText().data());
                auto other_value = quantity - new_value;
                left_text_box->SetText(std::to_string(other_value));
            }
        };
        AddGuiComponent(left_text_box);
        AddGuiComponent(right_text_box);
        auto split_action = [=] {
            auto right_value = std::stoi(right_text_box->GetText().data());
            object_->AlterQuantity(-right_value);
            for (auto i = 0; i < 1000; i++) {
                if (Player::Get()->data_.inventory_.objects_.count(i) == 0) {
                    Player::Get()->data_.inventory_.objects_.insert(
                        {i, std::make_shared<Object>(object_->GetObjectType(), right_value)});
                    return;
                }
            }
        };
        auto confirm_action = [=, this] {
            split_action();
            ActionRepeat::Get()->SetRepeatingAction(split_action);
            auto play_scene = PlayScene::Get();
            auto scene_gui = play_scene->GetSceneGui();
            scene_gui->RemoveGuiComponent(this);
        };
        AddGuiButton("Ok", {0.12f, 0.11f, 0.035f, ConvertWidthToHeight(0.015f)}, confirm_action);
    }

    void SplitStackGui::UpdateDerived() {
    }

    void SplitStackGui::RenderDerived() const {
    }
}