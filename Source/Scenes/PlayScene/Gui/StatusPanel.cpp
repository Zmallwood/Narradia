//////////////////////////////////////////////////////////////////////
#include "StatusPanel.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    StatusPanel::StatusPanel()
    /*//////////////////////*/
    {
        rendIdBgImage = Renderer2DImages::Get().NewImage();
        std::tie(rendIdPlHpText, rendIdPlStamText, rendIdPlRageText) = TextRenderer::Get().NewStrings<3>();
        std::tie(rendIdPlHpBar, rendIdPlStamBar, rendIdPlRageBar) = Renderer2DSolidColors::Get().NewRectangles<3>();
        rendIdPlHpBarFilled = Renderer2DImages::Get().NewImage();
        rendIdPlStamBarFilled = Renderer2DImages::Get().NewImage();
        rendIdPlRageBarFilled = Renderer2DImages::Get().NewImage();
    }

    void
    StatusPanel::Update()
    /*//////////////////////*/
    {
    }

    void
    StatusPanel::Render() const
    /*////////////////////////////*/
    {
        auto player = Player::GetPointer();
        Renderer2DImages::Get().DrawImage(hashPanelBg, rendIdBgImage, boundsPanel);
        auto tGfx = TextRenderer::GetPointer();
        tGfx->DrawString(rendIdPlHpText, lblTextHealth.data(), {0.01f, 0.03f});
        tGfx->DrawString(rendIdPlStamText, lblTextStamina.data(), {0.01f, 0.06f});
        tGfx->DrawString(rendIdPlRageText, lblTextRage.data(), {0.01f, 0.09f});
        auto hpBarFilledBounds = hpBarBounds.WithWidth(
            ((float)player->data.stats.hp) / player->data.stats.maxHp * hpBarBounds.width);
        auto stamBarFilledBounds = stamBarBounds.WithWidth(
            ((float)player->data.stats.stam) / player->data.stats.maxStam * stamBarBounds.width);
        auto rageBarFilledBounds = rageBarBounds.WithWidth(
            ((float)player->data.stats.rage) / player->data.stats.maxRage * rageBarBounds.width);
        auto rCol2D = Renderer2DSolidColors::GetPointer();
        rCol2D->FillRectangle(rendIdPlHpBar, hpBarBounds, Colors::black);
        Renderer2DImages::Get().DrawImage("Green", rendIdPlHpBarFilled, hpBarFilledBounds);
        rCol2D->FillRectangle(rendIdPlStamBar, stamBarBounds, Colors::black);
        Renderer2DImages::Get().DrawImage("Yellow", rendIdPlStamBarFilled, stamBarFilledBounds);
        rCol2D->FillRectangle(rendIdPlRageBar, rageBarBounds, Colors::black);
        Renderer2DImages::Get().DrawImage("Red", rendIdPlRageBarFilled, rageBarFilledBounds);
    }
}
//////////////////////////////////////////////////////////////////////