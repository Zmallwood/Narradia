#include "StatusPanel.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    StatusPanel::StatusPanel() {
        rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        std::tie(rendid_health_text_, rendid_stamina_text_, rendid_rage_text_) =
            TextRenderer::Get()->NewStrings<3>();
        std::tie(rendid_health_bar_, rendid_stamina_bar_, rendid_rage_bar_) =
            Renderer2DSolidColors::Get()->NewRectangles<3>();
        rendid_health_bar_filled_ = Renderer2DImages::Get()->NewImage();
        rendid_stamina_bar_filled_ = Renderer2DImages::Get()->NewImage();
        rendid_rage_bar_filled_ = Renderer2DImages::Get()->NewImage();
    }

    void StatusPanel::Update() {
    }

    void StatusPanel::Render() const {
        auto player = Player::Get();
        Renderer2DImages::Get()->DrawImage(
            kHashPanelBackground, rendid_background_image_, kBoundsPanel);
        auto text_renderer = TextRenderer::Get();
        text_renderer->DrawString(rendid_health_text_, kLabelTextHealth.data(), {0.01f, 0.03f});
        text_renderer->DrawString(rendid_stamina_text_, kLabelTextStamina.data(), {0.01f, 0.06f});
        text_renderer->DrawString(rendid_rage_text_, kLabelTextRage.data(), {0.01f, 0.09f});
        auto hp_bar_filled_bounds = kHpBarBounds.WithWidth(
            ((float)player->data_.status_.health) / player->data_.status_.max_health *
            kHpBarBounds.width);
        auto stamina_bar_filled_bounds = kStaminaBarBounds.WithWidth(
            ((float)player->data_.status_.stamina) / player->data_.status_.max_stamina *
            kStaminaBarBounds.width);
        auto rage_bar_filled_bounds = kRageBarBounds.WithWidth(
            ((float)player->data_.status_.rage) / player->data_.status_.max_rage *
            kRageBarBounds.width);
        auto renderer_2d_solid_colors = Renderer2DSolidColors::Get();
        renderer_2d_solid_colors->FillRectangle(rendid_health_bar_, kHpBarBounds, Colors::black);
        Renderer2DImages::Get()->DrawImage(
            "Green", rendid_health_bar_filled_, hp_bar_filled_bounds);
        renderer_2d_solid_colors->FillRectangle(
            rendid_stamina_bar_, kStaminaBarBounds, Colors::black);
        Renderer2DImages::Get()->DrawImage(
            "Yellow", rendid_stamina_bar_filled_, stamina_bar_filled_bounds);
        renderer_2d_solid_colors->FillRectangle(rendid_rage_bar_, kRageBarBounds, Colors::black);
        Renderer2DImages::Get()->DrawImage("Red", rendid_rage_bar_filled_, rage_bar_filled_bounds);
    }
}