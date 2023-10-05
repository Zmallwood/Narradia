//////////////////////////////////////////////////////////////////////
#include "SplitStackGui.hpp"
#include "Scenes/PlayScene/Modules/PlaySceneModulesCore.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "World/Object.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    SplitStackGui::SplitStackGui(Object *object_)
        : GuiWindow("Split", {0.15f, 0.2f, 0.17f, 0.15f}),
          object(object_)
    /*//////////////////////////////////////////////////*/
    {
        Show();
        auto quantity = object_->GetQuantity();
        auto text = std::to_string(quantity / 2);
        auto leftTbChangeEvent = std::make_shared<std::function<void()>>();
        auto rightTbChangeEvent = std::make_shared<std::function<void()>>();
        auto leftTextBox = std::make_shared<GuiTextBox>(
            RectangleF{0.01f, 0.05f, 0.05f, 0.03f}, this, text, leftTbChangeEvent);
        auto rightTextBox = std::make_shared<GuiTextBox>(
            RectangleF{0.11f, 0.05f, 0.05f, 0.03f}, this, text, rightTbChangeEvent);
        *leftTbChangeEvent = [=]
        /**********************/ {
            if (leftTextBox->GetText() == "")
            /*******************************/ {
                rightTextBox->SetText(std::to_string(quantity));
            } else
            /****/ {
                auto newValue = std::stoi(leftTextBox->GetText().data());
                auto otherValue = quantity - newValue;
                rightTextBox->SetText(std::to_string(otherValue));
            }
        };
        *rightTbChangeEvent = [=]
        /***********************/ {
            if (rightTextBox->GetText() == "")
            /********************************/ {
                leftTextBox->SetText(std::to_string(quantity));
            } else
            /****/ {
                auto newValue = std::stoi(rightTextBox->GetText().data());
                auto otherValue = quantity - newValue;
                leftTextBox->SetText(std::to_string(otherValue));
            }
        };
        AddGuiComponent(leftTextBox);
        AddGuiComponent(rightTextBox);
        auto splitAction = [=]
        /********************/ {
            auto leftValue = std::stoi(leftTextBox->GetText().data());
            auto rightValue = std::stoi(rightTextBox->GetText().data());
            object_->AlterQuantity(-rightValue);
            for (auto i = 0; i < 1000; i++)
            /*****************************/ {
                if (Player::Get().data.inventory.objects.count(i) == 0)
                /***************************************************/ {
                    Player::Get().data.inventory.objects.insert(
                        {i, std::make_shared<Object>(object_->GetObjectType(), rightValue)});
                    return;
                }
            }
        };
        auto confirmAction = [=, this]
        /****************************/ {
            splitAction();
            ActionRepeat::Get().SetRepeatingAction(splitAction);
            auto mainScene = PlayScene::GetPointer();
            auto sceneGui = mainScene->GetSceneGui();
            sceneGui->RemoveGuiComponent(this);
        };
        AddGuiButton("Ok", {0.12f, 0.11f, 0.035f, ConvertWidthToHeight(0.015f)}, confirmAction);
    }

    void SplitStackGui::UpdateDerived()
    /*//////////////////////////*/
    {
    }

    void SplitStackGui::RenderDerived() const
    /*////////////////////////////////*/
    {
    }
}
//////////////////////////////////////////////////////////////////////