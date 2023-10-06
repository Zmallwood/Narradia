//////////////////////////////////////////////////////////////////////
#include "StatusPanel.hpp"
#include "Engine/Rendering.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    StatusPanel::StatusPanel()
    /*//////////////////////*/
    {
        rendIdBgImage = Renderer2DImages::Get().NewImage();
        std::tie(rendIdPlHpText, rendIdPlStamText) = TextRenderer::Get().NewStrings<2>();
        std::tie(rendIdPlHpBar, rendIdPlStamBar) = Renderer2DSolidColors::Get().NewRectangles<2>();
        rendIdPlHpBarFilled = Renderer2DImages::Get().NewImage();
        rendIdPlStamBarFilled = Renderer2DImages::Get().NewImage();
    }

    void StatusPanel::Update()
    /*//////////////////////*/
    {
    }

    void StatusPanel::Render() const
    /*////////////////////////////*/
    {
        auto player = Player::GetPointer();
        Renderer2DImages::Get().DrawImage(hashPanelBg, rendIdBgImage, boundsPanel);
        auto tGfx = TextRenderer::GetPointer();
        tGfx->DrawString(rendIdPlHpText, lblTextHealth.data(), {0.01f, 0.03f});
        tGfx->DrawString(rendIdPlStamText, lblTextStamina.data(), {0.01f, 0.06f});
        auto hpBarFilledBounds = hpBarBounds.WithWidth(
            ((float)player->data.stats.hp) / player->data.stats.maxHp * hpBarBounds.width);
        auto stamBarFilledBounds = stamBarBounds.WithWidth(
            ((float)player->data.stats.stam) / player->data.stats.maxStam * stamBarBounds.width);
        auto rCol2D = Renderer2DSolidColors::GetPointer();
        rCol2D->FillRectangle(rendIdPlHpBar, hpBarBounds, Colors::black);
        Renderer2DImages::Get().DrawImage("Red", rendIdPlHpBarFilled, hpBarFilledBounds);
        rCol2D->FillRectangle(rendIdPlStamBar, stamBarBounds, Colors::black);
        Renderer2DImages::Get().DrawImage("Yellow", rendIdPlStamBarFilled, stamBarFilledBounds);
    }
}
//////////////////////////////////////////////////////////////////////