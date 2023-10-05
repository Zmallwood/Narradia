//////////////////////////////////////////////////////////////////////
#include "BasicScenes.hpp"
#include "Core/GuiCore.hpp"
#include "Scenes/Shared/MapFilesIO.hpp"
#include "Core/Rendering.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class IntroScene::Pimpl
    /*//////////////////*/ {
      public:
        RenderId rendIdBackImage;
        RenderId rendIdLogoImage;
        RenderId rendIdStartText;
    }; // Class

    IntroScene::IntroScene()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        CreateGui();
        p->rendIdBackImage = Renderer2DImages::Get().NewImage();
        p->rendIdLogoImage = Renderer2DImages::Get().NewImage();
        p->rendIdStartText = TextRenderer::Get().NewString();
        Audio::Get().PlayMusic("ForestSounds");
    } // Function

    void IntroScene::UpdateDerived()
    /*///////////////////////////*/ {
        Log();
        MouseInput::Get().GetLeftButton().AddFiredAction(
            "IntroSceneGoToMainMenu", [&]() { SceneManager::Get().ChangeView(Scenes::MainMenu); },
            0);
        MouseInput::Get().GetMiddleButton().AddFiredAction(
            "IntroSceneGoToMainMenu", [&]() { SceneManager::Get().ChangeView(Scenes::MainMenu); },
            0);
        MouseInput::Get().GetRightButton().AddFiredAction(
            "IntroSceneGoToMainMenu", [&]() { SceneManager::Get().ChangeView(Scenes::MainMenu); },
            0);
        if (KeyboardInput::Get().AnyKeyHasBeenFired())
            SceneManager::Get().ChangeView(Scenes::MainMenu);
    } // Function

    void IntroScene::RenderDerived()
    /*///////////////////////////*/ {
        Log();
        RectangleF rectBack = {0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", p->rendIdBackImage, rectBack);
        RectangleF rectLogo = {0.3f, 0.1f, 0.4f, 0.1f};
        Renderer2DImages::Get().DrawImage("NarradiaLogo", p->rendIdLogoImage, rectLogo);

        if (SDL_GetTicks() % 1000 < 500)
            TextRenderer::Get().DrawString(
                p->rendIdStartText, "Press to Start", {0.5f, 0.5f}, Colors::wheat, true);
    } // Function

    class MainMenuScene::Pimpl
    /*/////////////////////*/ {
      public:
        const RectangleF rectBack = {0.0f, 0.0f, 1.0f, 1.0f};
        const RectangleF rectLogo = {0.4f, 0.1f, 0.2f, 0.05f};
        RenderId glIdBackImage;
        RenderId glIdLogoImage;
    }; // Class

    MainMenuScene::MainMenuScene()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        CreateGui();
        auto sceneGui = GetSceneGui();
        sceneGui->AddGuiButton("New game", {0.45f, 0.3f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeView(Scenes::MapTypeSelection);
        });
        sceneGui->AddGuiButton("Load game", {0.45f, 0.38f, 0.1f, 0.05f}, [=]() {});
        sceneGui->AddGuiButton("Map editor", {0.45f, 0.46f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeView(Scenes::Editor);
        });
        sceneGui->AddGuiButton(
            "Exit", {0.45f, 0.54f, 0.1f, 0.05f}, [=]() { GameEngine::Get().StopGame(); });
        p->glIdBackImage = Renderer2DImages::Get().NewImage();
        p->glIdLogoImage = Renderer2DImages::Get().NewImage();
    } // Function

    void MainMenuScene::UpdateDerived()
    /*//////////////////////////////*/ { Log(); } // Function

    void MainMenuScene::RenderDerived()
    /*//////////////////////////////*/ {
        Log();
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", p->glIdBackImage, p->rectBack);
        Renderer2DImages::Get().DrawImage("NarradiaLogo", p->glIdLogoImage, p->rectLogo);
    } // Function

    class MapTypeSelectionScene::Pimpl
    /*/////////////////////////////*/ {
      public:
        const RectangleF rectBack = {0.0f, 0.0f, 1.0f, 1.0f};
        const RectangleF rectLogo = {0.4f, 0.1f, 0.2f, 0.05f};
        RenderId glIdBackImage;
        RenderId glIdLogoImage;
    }; // Class

    MapTypeSelectionScene::MapTypeSelectionScene()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////*/ {
        CreateGui();
        auto sceneGui = GetSceneGui();
        sceneGui->AddGuiButton("Random wilderness map", {0.45f, 0.3f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeView(Scenes::MapGeneration);
        });
        sceneGui->AddGuiButton("User created map", {0.45f, 0.38f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeView(Scenes::UserMapSelection);
        });
        sceneGui->AddGuiButton("Back", {0.45f, 0.46f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeView(Scenes::MainMenu);
        });
        p->glIdBackImage = Renderer2DImages::Get().NewImage();
        p->glIdLogoImage = Renderer2DImages::Get().NewImage();
    } // Function

    void MapTypeSelectionScene::UpdateDerived()
    /*//////////////////////////////////////*/ { Log(); } // Function

    void MapTypeSelectionScene::RenderDerived()
    /*//////////////////////////////////////*/ {
        Log();
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", p->glIdBackImage, p->rectBack);
        Renderer2DImages::Get().DrawImage("NarradiaLogo", p->glIdLogoImage, p->rectLogo);
    } // Function

    UserMapSelectionScene::UserMapSelectionScene()
    /*////////////////////////////////////////*/ {
        CreateGui();
        glIdBackgroundImage = Renderer2DImages::Get().NewImage();
        auto bounds = RectangleF{0.45f, 0.4f, 0.1f, 0.035f};
        auto textBox = std::make_shared<GuiTextBox>(bounds, nullptr);
        GetSceneGui()->AddGuiComponent(textBox);
        GetSceneGui()->AddGuiButton("Load", {0.56f, 0.4f, 0.04f, 0.04f}, [=] {
            MapFilesIO::Get().LoadMapArea(textBox->GetText());
            SceneManager::Get().ChangeView(Scenes::Play);
        });
        GetSceneGui()->AddGuiButton("Return", {0.02f, 0.02f, 0.04f, 0.04f}, [] {
            SceneManager::Get().ChangeView(Scenes::MapTypeSelection);
        });
    } // Function

    void UserMapSelectionScene::UpdateDerived()
    /*//////////////////////////////////////*/ {} // Function

    void UserMapSelectionScene::RenderDerived()
    /*//////////////////////////////////////*/ {
        auto rectBack = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", glIdBackgroundImage, rectBack);
    } // Function
}
//////////////////////////////////////////////////////////////////////