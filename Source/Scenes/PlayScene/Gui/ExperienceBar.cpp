#include "ExperienceBar.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ExperienceBar::ExperienceBar()
    /*//////////////////////////*/
    {
        rendid_unfilled_area_ = Renderer2DSolidColors::Get()->NewRectangle();
        rendid_filled_area_ = Renderer2DImages::Get()->NewImage();
    }

    void
    ExperienceBar::Update()
    /*///////////////////*/
    {
    }

    void
    ExperienceBar::Render() const
    /*/////////////////////////*/
    {
        auto unfilled_area = RectangleF{0.0f, 1.0f - kBarHeight, 1.0f, kBarHeight};
        Renderer2DSolidColors::Get()->FillRectangle(rendid_unfilled_area_, unfilled_area, Colors::black);
        auto experience_progress = GetFractionalExpProgress(Player::Get()->data.experience_);
        auto filled_area = RectangleF{0.0f, 1.0f - kBarHeight, experience_progress, kBarHeight};
        Renderer2DImages::Get()->DrawImage("Green", rendid_filled_area_, filled_area);
    }
}