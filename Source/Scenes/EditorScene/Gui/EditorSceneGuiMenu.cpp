#include "EditorSceneGuiMenu.hpp"
#include "Engine/Core/GameEngine.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/SceneManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    EditorSceneGuiMenu::EditorSceneGuiMenu()
        : GuiContainer({0.0f, 0.0f}, {1.0f, 1.0f}) {
        rendid_canvas_cover_ = Renderer2DSolidColors::Get()->NewRectangle();
        AddGuiButton("Resume", {0.45f, 0.3f, 0.1f, 0.03f}, [] {
            EditorSceneGuiMenu::Get()->visible = false;
        });
        AddGuiButton("Save map", {0.45f, 0.35f, 0.1f, 0.03f}, [] {
            SceneManager::Get()->ChangeScene(SceneNames::MapSave);
        });
        AddGuiButton("Load map", {0.45f, 0.40f, 0.1f, 0.03f}, [] {
            SceneManager::Get()->ChangeScene(SceneNames::MapLoad);
        });
        AddGuiButton("Go to main menu", {0.45f, 0.45f, 0.1f, 0.03f}, [] {
            SceneManager::Get()->ChangeScene(SceneNames::MainMenu);
        });
        AddGuiButton(
            "Exit game", {0.45f, 0.50f, 0.1f, 0.03f}, [] { GameEngine::Get()->StopGame(); });
    }

    void EditorSceneGuiMenu::Update() {
        if (visible)
            GuiContainer::Update();
    }

    void EditorSceneGuiMenu::Render() const {
        if (!visible)
            return;
        auto rect = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        auto color = Color{0.3f, 0.6f, 1.0f, 0.5f};
        Renderer2DSolidColors::Get()->FillRectangle(rendid_canvas_cover_, rect, color);
        GuiContainer::Render();
    }
}