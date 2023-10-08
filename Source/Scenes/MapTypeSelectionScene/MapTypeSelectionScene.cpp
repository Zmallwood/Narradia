#include "MapTypeSelectionScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapTypeSelectionScene::Pimpl {
      public:
        const RectangleF kRectBackground = {0.0f, 0.0f, 1.0f, 1.0f};
        const RectangleF kRectLogo = {0.4f, 0.1f, 0.2f, 0.05f};
        RenderId rendid_background_image_;
        RenderId rendid_logo_image_;
    };

    MapTypeSelectionScene::MapTypeSelectionScene()
        : p(std::make_shared<Pimpl>()) {
        CreateGui();
        auto scene_gui = GetSceneGui();
        scene_gui->AddGuiButton("Random wilderness map", {0.45f, 0.3f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::MapGeneration);
        });
        scene_gui->AddGuiButton("User created map", {0.45f, 0.38f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::UserMapSelection);
        });
        scene_gui->AddGuiButton("Back", {0.45f, 0.46f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::MainMenu);
        });
        p->rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_logo_image_ = Renderer2DImages::Get()->NewImage();
    }

    void MapTypeSelectionScene::UpdateDerived() {
        Log();
    }

    void MapTypeSelectionScene::RenderDerived() {
        Log();
        Renderer2DImages::Get()->DrawImage(
            "DefaultSceneBackground", p->rendid_background_image_, p->kRectBackground);
        Renderer2DImages::Get()->DrawImage("NarradiaLogo", p->rendid_logo_image_, p->kRectLogo);
    }
}