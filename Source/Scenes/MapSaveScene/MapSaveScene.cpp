#include "MapSaveScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Engine/GuiCore/GuiTextBox.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/SharedSceneModules/MapFilesIO.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    MapSaveScene::MapSaveScene() {
        CreateGui();
        rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        auto bounds = RectangleF{0.45f, 0.4f, 0.1f, 0.035f};
        auto text_box = std::make_shared<GuiTextBox>(bounds, nullptr);
        GetSceneGui()->AddGuiComponent(text_box);
        GetSceneGui()->AddGuiButton("Save", {0.56f, 0.4f, 0.04f, 0.04f}, [=] {
            MapFilesIO::Get()->SaveCurrentMapArea(text_box->GetText());
        });
        GetSceneGui()->AddGuiButton("Return", {0.02f, 0.02f, 0.04f, 0.04f}, [] {
            SceneManager::Get()->ChangeScene(SceneNames::Editor);
        });
    }

    void MapSaveScene::UpdateDerived() {
    }

    void MapSaveScene::RenderDerived() {
        auto rect_bacground_image = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get()->DrawImage(
            "DefaultSceneBackground", rendid_background_image_, rect_bacground_image);
    }
}