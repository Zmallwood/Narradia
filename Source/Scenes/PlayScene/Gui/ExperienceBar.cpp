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
        glIdUnfilledArea = Renderer2DSolidColors::Get()->NewRectangle();
        glIdFilledArea = Renderer2DImages::Get()->NewImage();
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
        auto unfilledArea = RectangleF{0.0f, 1.0f - kBarHeight, 1.0f, kBarHeight};
        Renderer2DSolidColors::Get()->FillRectangle(glIdUnfilledArea, unfilledArea, Colors::black);
        auto expProgress = GetFractionalExpProgress(Player::Get()->data.exp);
        auto filledArea = RectangleF{0.0f, 1.0f - kBarHeight, expProgress, kBarHeight};
        Renderer2DImages::Get()->DrawImage("Green", glIdFilledArea, filledArea);
    }
}