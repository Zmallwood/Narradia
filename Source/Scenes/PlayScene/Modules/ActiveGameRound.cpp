//////////////////////////////////////////////////////////////////////
#include "ActiveGameRound.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Player.hpp"
#include "World/MapArea.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ActiveGameRound::Pimpl
    /*////////////////////////*/
    {
      public:
        bool game_completed = false;
        RenderId id_canvas_layer;
        RenderId id_game_result_text;
    };

    ActiveGameRound::ActiveGameRound()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        p->id_canvas_layer = Renderer2DSolidColors::Get().NewRectangle();
        p->id_game_result_text = TextRenderer::Get().NewString();
    }

    void
    ActiveGameRound::Update()
    /*//////////////////////////*/
    {
        auto num_claimed_tiles = Player::Get().claimedTiles.size();
        auto tot_number_of_tiles = MapArea::GetMapSize().width * MapArea::GetMapSize().height;
        auto progress = (int)(((float)num_claimed_tiles) / tot_number_of_tiles * 100);
        if (progress >= 51)
            p->game_completed = true;
    }

    void
    ActiveGameRound::Render()
    /*//////////////////////////*/
    {
        if (!p->game_completed)
            return;
        RectangleF rect = {0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DSolidColors::Get().FillRectangle(
            p->id_canvas_layer, rect, Colors::alphaElegantBlue);
        TextRenderer::Get().DrawString(
            p->id_game_result_text, "Congratulations! You have won the game", {0.5f, 0.3f},
            Colors::green, true, FontSizes::_26);
    }

    bool
    ActiveGameRound::GameIsCompleted()
    /*///////////////////////////////////*/
    {
        return p->game_completed;
    }
}
//////////////////////////////////////////////////////////////////////