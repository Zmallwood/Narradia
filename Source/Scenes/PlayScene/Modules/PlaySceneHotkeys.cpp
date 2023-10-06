//////////////////////////////////////////////////////////////////////
#include "PlaySceneHotkeys.hpp"
#include "Engine/Core/Input/KeyboardInput.hpp"
#include "Scenes/PlayScene/Gui/CharacterGui.hpp"
#include "Scenes/PlayScene/Gui/InventoryGui.hpp"
#include "Scenes/PlayScene/Gui/SkillsGui.hpp"
#include "World/Player.hpp"
#include "Scenes/PlayScene/Gui/PlaySceneGuiMenu.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    PlaySceneHotkeys::Update()
    /*//////////////////////////*/
    {
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_c))
            CharacterGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_b))
            InventoryGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_v))
            SkillsGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_z))
            Player::Get().mounted = !Player::Get().mounted;
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_SPACE))
            Player::Get().Jump();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_ESCAPE))
            PlaySceneGuiMenu::Get().visible = !PlaySceneGuiMenu::Get().visible;
    }
}
//////////////////////////////////////////////////////////////////////