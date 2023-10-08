#include "PlaySceneGuiMenu.hpp"
#include "Engine/Core/GameEngine.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/SceneManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    PlaySceneGuiMenu::PlaySceneGuiMenu()
        : GuiContainer({0.0f, 0.0f}, {1.0f, 1.0f}) {
        rendid_canvas_cover_ = Renderer2DSolidColors::Get()->NewRectangle();
        AddGuiButton(
            "Resume", {0.45f, 0.3f, 0.1f, 0.03f},
            []
            /**************************************************/
            { PlaySceneGuiMenu::Get()->visible_ = false; });
        AddGuiButton(
            "Save game", {0.45f, 0.35f, 0.1f, 0.03f},
            []
            /**************************************************/
            { SceneManager::Get()->ChangeScene(SceneNames::GameSave); });
        AddGuiButton(
            "Go to main menu", {0.45f, 0.4f, 0.1f, 0.03f},
            []
            /**************************************************/
            { SceneManager::Get()->ChangeScene(SceneNames::MainMenu); });
        AddGuiButton(
            "Exit game", {0.45f, 0.45f, 0.1f, 0.03f},
            []
            /**************************************************/
            { GameEngine::Get()->StopGame(); });
    }

    void PlaySceneGuiMenu::Update() {
        if (visible_)
            GuiContainer::Update();
    }

    void PlaySceneGuiMenu::Render() const {
        if (!visible_)
            return;
        auto rect = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        auto color = Color{0.3f, 0.6f, 1.0f, 0.5f};
        Renderer2DSolidColors::Get()->FillRectangle(rendid_canvas_cover_, rect, color);
        GuiContainer::Render();
    }
}