#include "MainMenuScene.hpp"
#include "Engine/Core/GameEngine.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MainMenuScene::Pimpl
    /*//////////////////////*/
    {
      public:
        const RectangleF kRectBackground = {0.0f, 0.0f, 1.0f, 1.0f};
        const RectangleF kRectLogo = {0.4f, 0.1f, 0.2f, 0.05f};
        RenderId rendid_background_image_;
        RenderId rendid_logo_image_;
    };

    MainMenuScene::MainMenuScene()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        CreateGui();
        auto scene_gui = GetSceneGui();
        scene_gui->AddGuiButton("New game", {0.45f, 0.3f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::MapTypeSelection);
        });
        scene_gui->AddGuiButton("Load game", {0.45f, 0.38f, 0.1f, 0.05f}, [=]() {});
        scene_gui->AddGuiButton("Map editor", {0.45f, 0.46f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::Editor);
        });
        scene_gui->AddGuiButton(
            "Exit", {0.45f, 0.54f, 0.1f, 0.05f}, [=]() { GameEngine::Get()->StopGame(); });
        p->rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_logo_image_ = Renderer2DImages::Get()->NewImage();
    }

    void
    MainMenuScene::UpdateDerived()
    /*//////////////////////////*/
    {
        Log();
    }

    void
    MainMenuScene::RenderDerived()
    /*//////////////////////////*/
    {
        Log();
        Renderer2DImages::Get()->DrawImage("DefaultSceneBackground", p->rendid_background_image_, p->kRectBackground);
        Renderer2DImages::Get()->DrawImage("NarradiaLogo", p->rendid_logo_image_, p->kRectLogo);
    }
}