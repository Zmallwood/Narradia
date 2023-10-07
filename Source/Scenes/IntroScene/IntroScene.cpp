#include "IntroScene.hpp"
#include "Engine/Core/Audio.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Input/KeyboardInput.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/Core/SceneManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class IntroScene::Pimpl
    /*///////////////////*/
    {
      public:
        RenderId rendid_background_image_;
        RenderId rendid_logo_image_;
        RenderId rendid_start_text_;
    };

    IntroScene::IntroScene()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        CreateGui();
        p->rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_logo_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_start_text_ = TextRenderer::Get()->NewString();
        Audio::Get()->PlayMusic("ForestSounds");
    }

    void
    IntroScene::UpdateDerived()
    /*///////////////////////*/
    {
        Log();
        MouseInput::Get()->GetLeftButton().AddFiredAction(
            "IntroSceneGoToMainMenu",
            [&]() { SceneManager::Get()->ChangeScene(SceneNames::MainMenu); }, 0);
        MouseInput::Get()->GetMiddleButton().AddFiredAction(
            "IntroSceneGoToMainMenu",
            [&]() { SceneManager::Get()->ChangeScene(SceneNames::MainMenu); }, 0);
        MouseInput::Get()->GetRightButton().AddFiredAction(
            "IntroSceneGoToMainMenu",
            [&]() { SceneManager::Get()->ChangeScene(SceneNames::MainMenu); }, 0);
        if (KeyboardInput::Get()->AnyKeyHasBeenFired())
            SceneManager::Get()->ChangeScene(SceneNames::MainMenu);
    }

    void
    IntroScene::RenderDerived()
    /*///////////////////////*/
    {
        Log();
        RectangleF rect_background_image = {0.0f, 0.0f, 1.0f, 1.0f};
        Log();
        Renderer2DImages::Get()->DrawImage("DefaultSceneBackground", p->rendid_background_image_, rect_background_image);
        Log();
        RectangleF rect_logo = {0.3f, 0.1f, 0.4f, 0.1f};
        Renderer2DImages::Get()->DrawImage("NarradiaLogo", p->rendid_logo_image_, rect_logo);
        Log();
        if (SDL_GetTicks() % 1000 < 500)
            TextRenderer::Get()->DrawString(
                p->rendid_start_text_, "Press to Start", {0.5f, 0.5f}, Colors::wheat, true);
        Log();
    }
}