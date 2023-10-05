//////////////////////////////////////////////////////////////////////
#include "PlaySceneModulesCore.hpp"
#include "Scenes/PlayScene/Gui/CharacterGui.hpp"
#include "Scenes/PlayScene/Gui/InventoryGui.hpp"
#include "Scenes/PlayScene/Gui/PlaySceneGuiMenu.hpp"
#include "Core/Rendering.hpp"
#include "Scenes/PlayScene/Gui/SkillsGui.hpp"
#include "World/Player.hpp"
#include "World/MapArea.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void PlaySceneHotkeys::Update()
    /*//////////////////////////*/ {
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_c))
            CharacterGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_b))
            InventoryGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_v))
            SkillsGui::Get().ToggleVisibility();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_z))
            Player::Get().mounted = !Player::Get().mounted;
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_SPACE)) Player::Get().Jump();
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_ESCAPE))
            PlaySceneGuiMenu::Get().visible = !PlaySceneGuiMenu::Get().visible;
    } // Function

    class ActionRepeat::Pimpl
    /*/////////////////////*/ {
      public:
        std::function<void()> repeatingAction = nullptr;
    }; // Class

    ActionRepeat::ActionRepeat()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void ActionRepeat::Update()
    /*///////////////////////*/ {
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_r)) RepeatLastAction();
    } // Function

    void ActionRepeat::RepeatLastAction()
    /*/////////////////////////////////*/ {
        if (p->repeatingAction == nullptr)
        /********************************/ {
            TextOutBox::Get().Print("There is no previous action to repeat.");
        } else
        /****/ {
            TextOutBox::Get().Print("Repeating action.");
            Log();
            p->repeatingAction();
        }
    } // Function

    void ActionRepeat::SetRepeatingAction(std::function<void()> action)
    /*///////////////////////////////////////////////////////////////*/ {
        p->repeatingAction = action;
    } // Function

    class ActiveGameRound::Pimpl
    /*////////////////////////*/ {
      public:
        bool game_completed = false;
        RenderId id_canvas_layer;
        RenderId id_game_result_text;
    }; // Class

    ActiveGameRound::ActiveGameRound()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        p->id_canvas_layer = Renderer2DSolidColors::Get().NewRectangle();
        p->id_game_result_text = TextRenderer::Get().NewString();
    } // Function

    void ActiveGameRound::Update()
    /*//////////////////////////*/ {
        auto num_claimed_tiles = Player::Get().claimedTiles.size();
        auto tot_number_of_tiles = MapArea::GetMapSize().width * MapArea::GetMapSize().height;
        auto progress = (int)(((float)num_claimed_tiles) / tot_number_of_tiles * 100);
        if (progress >= 51) p->game_completed = true;
    } // Function

    void ActiveGameRound::Render()
    /*//////////////////////////*/ {
        if (!p->game_completed) return;
        RectangleF rect = {0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DSolidColors::Get().FillRectangle(
            p->id_canvas_layer, rect, Colors::alphaElegantBlue);
        TextRenderer::Get().DrawString(
            p->id_game_result_text, "Congratulations! You have won the game", {0.5f, 0.3f},
            Colors::green, true, TextSizes::_26);
    } // Function

    bool ActiveGameRound::GameIsCompleted()
    /*///////////////////////////////////*/ { return p->game_completed; } // Function
}
//////////////////////////////////////////////////////////////////////