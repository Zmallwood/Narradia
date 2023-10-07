//////////////////////////////////////////////////////////////////////
#include "UserMapSelectionScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Engine/GuiCore/GuiTextBox.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/SharedSceneModules/MapFilesIO.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    UserMapSelectionScene::UserMapSelectionScene()
    /*////////////////////////////////////////*/
    {
        CreateGui();
        glIdBackgroundImage = Renderer2DImages::Get().NewImage();
        auto bounds = RectangleF{0.45f, 0.4f, 0.1f, 0.035f};
        auto textBox = std::make_shared<GuiTextBox>(bounds, nullptr);
        GetSceneGui()->AddGuiComponent(textBox);
        GetSceneGui()->AddGuiButton("Load", {0.56f, 0.4f, 0.04f, 0.04f}, [=] {
            MapFilesIO::Get().LoadMapArea(textBox->GetText());
            SceneManager::Get().ChangeScene(SceneNames::Play);
        });
        GetSceneGui()->AddGuiButton("Return", {0.02f, 0.02f, 0.04f, 0.04f}, [] {
            SceneManager::Get().ChangeScene(SceneNames::MapTypeSelection);
        });
    }

    void
    UserMapSelectionScene::UpdateDerived()
    /*//////////////////////////////////////*/
    {
    }

    void
    UserMapSelectionScene::RenderDerived()
    /*//////////////////////////////////////*/
    {
        auto rectBack = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", glIdBackgroundImage, rectBack);
    }
}
//////////////////////////////////////////////////////////////////////