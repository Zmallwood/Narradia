#include "CompanionsPanel.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Companion.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    CompanionsPanel::CompanionsPanel()
        : GuiMovableContainer(kSize.ToRectangle(), kStartPosition, kSize),
          kRelBoundsHorizontalSplitter({0.005f, 0.035f, kSize.width - 2 * 0.005f, 0.002f}) {
        rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        rendid_horizontal_splitter_ = Renderer2DImages::Get()->NewImage();
        rendid_title_text_ = TextRenderer::Get()->NewString();
        rendids_companions_label_texts_[0] = TextRenderer::Get()->NewString();
        rendids_companions_label_texts_[1] = TextRenderer::Get()->NewString();
        rendids_companions_label_texts_[2] = TextRenderer::Get()->NewString();
        rendids_companions_label_texts_[3] = TextRenderer::Get()->NewString();
        rendids_companions_stamina_backs_[0] = Renderer2DSolidColors::Get()->NewRectangle();
        rendids_companions_stamina_backs_[1] = Renderer2DSolidColors::Get()->NewRectangle();
        rendids_companions_stamina_backs_[2] = Renderer2DSolidColors::Get()->NewRectangle();
        rendids_companions_stamina_backs_[3] = Renderer2DSolidColors::Get()->NewRectangle();
        rendids_companions_stamina_filleds_[0] = Renderer2DImages::Get()->NewImage();
        rendids_companions_stamina_filleds_[1] = Renderer2DImages::Get()->NewImage();
        rendids_companions_stamina_filleds_[2] = Renderer2DImages::Get()->NewImage();
        rendids_companions_stamina_filleds_[3] = Renderer2DImages::Get()->NewImage();
    }

    void CompanionsPanel::Update() {
        GuiMovableContainer::Update();
    }

    void CompanionsPanel::Render() const {
        Renderer2DImages::Get()->DrawImage(Hash("PanelBg"), rendid_background_image_, GetBounds());
        TextRenderer::Get()->DrawString(
            rendid_title_text_, kTitleText.data(), GetPosition().Translate(0.014f, 0.018f));
        Renderer2DImages::Get()->DrawImage(
            Hash("HorizontalSplitter"), rendid_horizontal_splitter_,
            kRelBoundsHorizontalSplitter.Translate(GetPosition().x, GetPosition().y));
        auto player_world_area_position = Player::Get()->GetWorldAreaPos();
        auto map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_position.z);
        auto it = map_area->GetCompanionsMirror().cbegin();
        for (auto i = 0; i < 4; i++) {
            auto companion = it->first;
            auto stamina = companion->GetStamina();
            auto max_stamina = companion->GetMaxStamina();
            auto fraction = static_cast<float>(stamina) / max_stamina;
            auto stamina_bar_width_back = 0.08f;
            auto stamina_bar_height = 0.015f;
            auto stamina_bar_width_filled = fraction * stamina_bar_width_back;
            auto rectangle_back = RectangleF{
                GetPosition().x + 0.05f, GetPosition().y + 0.065f + i * 0.04f,
                stamina_bar_width_back, stamina_bar_height};
            auto rectangle_filled = rectangle_back;
            rectangle_filled.width = stamina_bar_width_filled;
            TextRenderer::Get()->DrawString(
                rendids_companions_label_texts_[i], "Comp. " + std::to_string(i),
                GetPosition().Translate(0.01f, 0.07f + i * 0.04f));
            Renderer2DSolidColors::Get()->FillRectangle(
                rendids_companions_stamina_backs_[i], rectangle_back, Colors::black);
            Renderer2DImages::Get()->DrawImage(
                "Yellow", rendids_companions_stamina_filleds_[i], rectangle_filled);
            ++it;
        }
        GuiMovableContainer::Render();
    }
}