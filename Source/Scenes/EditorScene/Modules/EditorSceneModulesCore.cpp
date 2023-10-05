//////////////////////////////////////////////////////////////////////
#include "EditorSceneModulesCore.hpp"
#include "Scenes/EditorScene/Gui/EditorSceneGuiMenu.hpp"
#include "Scenes/EditorScene/Gui/GroundBrowserGui.hpp"
#include "Scenes/EditorScene/Gui/MobBrowserGui.hpp"
#include "Scenes/EditorScene/Gui/ObjectBrowserGui.hpp"
#include "Scenes/EditorScene/Gui/RoofBrowserGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void EditorSceneHotkeys::Update()
    /*////////////////////////////*/ {
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_1))
            GroundBrowserGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_2))
            ObjectBrowserGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_3))
            RoofBrowserGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_4))
            MobBrowserGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_ESCAPE))
            EditorSceneGuiMenu::Get().visible = !EditorSceneGuiMenu::Get().visible;
    } // Function
}
//////////////////////////////////////////////////////////////////////