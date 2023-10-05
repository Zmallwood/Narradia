//////////////////////////////////////////////////////////////////////
#include "MapLoadScene.hpp"
#include "Core/GuiCore.hpp"
#include "Core/Rendering.hpp"
#include "Scenes/Shared/MapFilesIO.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    MapLoadScene::MapLoadScene()
    /*//////////////////////*/ {
        CreateGui();
        glIdBackgroundImage = Renderer2DImages::Get().NewImage();
        auto bounds = RectangleF{0.45f, 0.4f, 0.1f, 0.035f};
        auto textBox = std::make_shared<GuiTextBox>(bounds, nullptr);
        GetSceneGui()->AddGuiComponent(textBox);
        GetSceneGui()->AddGuiButton("Load", {0.56f, 0.4f, 0.04f, 0.04f}, [=] {
            MapFilesIO::Get().LoadMapArea(textBox->GetText());
        });
        GetSceneGui()->AddGuiButton("Return", {0.02f, 0.02f, 0.04f, 0.04f}, [] {
            SceneManager::Get().ChangeView(Scenes::Editor);
        });
    }

    void MapLoadScene::UpdateDerived()
    /*/////////////////////////////*/ {
    }

    void MapLoadScene::RenderDerived()
    /*/////////////////////////////*/ {
        auto rectBack = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", glIdBackgroundImage, rectBack);
    }
}
//////////////////////////////////////////////////////////////////////