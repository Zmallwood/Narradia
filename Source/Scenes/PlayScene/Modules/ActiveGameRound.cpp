#include "ActiveGameRound.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ActiveGameRound::Pimpl
    /*////////////////////////*/
    {
      public:
        bool game_completed_ = false;
        RenderId rendid_canvas_layer_;
        RenderId rendid_game_result_text_;
    };

    ActiveGameRound::ActiveGameRound()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        p->rendid_canvas_layer_ = Renderer2DSolidColors::Get()->NewRectangle();
        p->rendid_game_result_text_ = TextRenderer::Get()->NewString();
    }

    void
    ActiveGameRound::Update()
    /*/////////////////////*/
    {
        auto num_claimed_tiles = Player::Get()->claimedTiles.size();
        auto tot_number_of_tiles = MapArea::GetMapSize().width * MapArea::GetMapSize().height;
        auto progress = (int)(((float)num_claimed_tiles) / tot_number_of_tiles * 100);
        if (progress >= 51)
            p->game_completed_ = true;
    }

    void
    ActiveGameRound::Render()
    /*/////////////////////*/
    {
        if (!p->game_completed_)
            return;
        RectangleF rect = {0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DSolidColors::Get()->FillRectangle(
            p->rendid_canvas_layer_, rect, Colors::alphaElegantBlue);
        TextRenderer::Get()->DrawString(
            p->rendid_game_result_text_, "Congratulations! You have won the game", {0.5f, 0.3f},
            Colors::green, true, FontSizes::_26);
    }

    bool
    ActiveGameRound::GameIsCompleted()
    /*//////////////////////////////*/
    {
        return p->game_completed_;
    }
}