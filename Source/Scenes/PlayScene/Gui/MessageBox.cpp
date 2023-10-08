#include "MessageBox.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    MessageBox::MessageBox(std::string_view title_, std::string_view messageText_)
        : GuiWindow(title_, {0.25f, 0.35f, 0.5f, 0.1f}, true, "PanelBgBlue"),
          message_text_(messageText_) {
        Show();
        rendid_message_text_ = TextRenderer::Get()->NewString();
        auto main_scene = PlayScene::Get();
        auto scene_gui = main_scene->GetSceneGui();
        std::function<void()> confirm_action = [=, this] { scene_gui->RemoveGuiComponent(this); };
        AddGuiButton("Ok", {0.45f, 0.06f, 0.035f, ConvertWidthToHeight(0.015f)}, confirm_action);
    }

    void MessageBox::UpdateDerived() {
    }

    void MessageBox::RenderDerived() const {
        TextRenderer::Get()->DrawString(
            rendid_message_text_, message_text_, GetPosition().Translate(0.01f, 0.05f),
            Colors::yellow);
    }
}