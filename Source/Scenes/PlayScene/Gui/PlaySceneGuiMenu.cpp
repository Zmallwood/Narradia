//////////////////////////////////////////////////////////////////////
#include "PlaySceneGuiMenu.hpp"
#include "Core/GuiCore.hpp"
#include "Core/Rendering.hpp"
#include "Scenes/PlayScene/Modules/PlaySceneModulesCore.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    PlaySceneGuiMenu::PlaySceneGuiMenu()
        : GuiContainer({0.0f, 0.0f}, {1.0f, 1.0f})
    /*//////////////////////////////////////////*/
    {
        glIdCanvasCover = Renderer2DSolidColors::Get().NewRectangle();
        AddGuiButton(
            "Resume", {0.45f, 0.3f, 0.1f, 0.03f},
            []
            /**************************************************/
            { PlaySceneGuiMenu::Get().visible = false; });
        AddGuiButton(
            "Save game", {0.45f, 0.35f, 0.1f, 0.03f},
            []
            /**************************************************/
            { SceneManager::Get().ChangeView(Scenes::GameSave); });
        AddGuiButton(
            "Go to main menu", {0.45f, 0.4f, 0.1f, 0.03f},
            []
            /**************************************************/
            { SceneManager::Get().ChangeView(Scenes::MainMenu); });
        AddGuiButton(
            "Exit game", {0.45f, 0.45f, 0.1f, 0.03f},
            []
            /**************************************************/
            { GameEngine::Get().StopGame(); });
    }

    void PlaySceneGuiMenu::Update()
    /*//////////////////////////*/
    {
        if (visible)
            GuiContainer::Update();
    }

    void PlaySceneGuiMenu::Render() const
    /*////////////////////////////////*/
    {
        if (!visible)
            return;
        auto rect = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        auto color = Color{0.3f, 0.6f, 1.0f, 0.5f};
        Renderer2DSolidColors::Get().FillRectangle(glIdCanvasCover, rect, color);
        GuiContainer::Render();
    }
}
//////////////////////////////////////////////////////////////////////