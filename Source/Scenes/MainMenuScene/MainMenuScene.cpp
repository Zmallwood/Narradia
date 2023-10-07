//////////////////////////////////////////////////////////////////////
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
        const RectangleF rectBack = {0.0f, 0.0f, 1.0f, 1.0f};
        const RectangleF rectLogo = {0.4f, 0.1f, 0.2f, 0.05f};
        RenderId glIdBackImage;
        RenderId glIdLogoImage;
    };

    MainMenuScene::MainMenuScene()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        CreateGui();
        auto sceneGui = GetSceneGui();
        sceneGui->AddGuiButton("New game", {0.45f, 0.3f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::MapTypeSelection);
        });
        sceneGui->AddGuiButton("Load game", {0.45f, 0.38f, 0.1f, 0.05f}, [=]() {});
        sceneGui->AddGuiButton("Map editor", {0.45f, 0.46f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get()->ChangeScene(SceneNames::Editor);
        });
        sceneGui->AddGuiButton(
            "Exit", {0.45f, 0.54f, 0.1f, 0.05f}, [=]() { GameEngine::Get()->StopGame(); });
        p->glIdBackImage = Renderer2DImages::Get()->NewImage();
        p->glIdLogoImage = Renderer2DImages::Get()->NewImage();
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
        Renderer2DImages::Get()->DrawImage("DefaultSceneBackground", p->glIdBackImage, p->rectBack);
        Renderer2DImages::Get()->DrawImage("NarradiaLogo", p->glIdLogoImage, p->rectLogo);
    }
}