//////////////////////////////////////////////////////////////////////
#include "MessageBox.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    MessageBox::MessageBox(std::string_view title_, std::string_view messageText_)
        : GuiWindow(title_, {0.25f, 0.35f, 0.5f, 0.1f}, true, "PanelBgBlue"),
          messageText(messageText_)
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        Show();
        glIdMessageText = TextRenderer::Get().NewString();
        auto mainScene = PlayScene::GetPointer();
        auto sceneGui = mainScene->GetSceneGui();
        std::function<void()> confirmAction = [=, this] { sceneGui->RemoveGuiComponent(this); };
        AddGuiButton("Ok", {0.45f, 0.06f, 0.035f, ConvertWidthToHeight(0.015f)}, confirmAction);
    }

    void
    MessageBox::UpdateDerived()
    /*////////////////////////////*/
    {
    }

    void
    MessageBox::RenderDerived() const
    /*//////////////////////////////////*/
    {
        TextRenderer::Get().DrawString(
            glIdMessageText, messageText, GetPosition().Translate(0.01f, 0.05f), Colors::yellow);
    }
}
//////////////////////////////////////////////////////////////////////