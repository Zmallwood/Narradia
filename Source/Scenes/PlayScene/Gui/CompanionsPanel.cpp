//////////////////////////////////////////////////////////////////////
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
        : GuiMovableContainer(k_size.ToRectangle(), k_start_position, k_size),
          rel_bounds_horizontal_splitter({0.005f, 0.035f, k_size.width - 2 * 0.005f, 0.002f})
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        id_background_image = Renderer2DImages::Get().NewImage();
        id_horizntal_splitter = Renderer2DImages::Get().NewImage();
        id_title_text = TextRenderer::Get().NewString();
        ids_companions_label_text[0] = TextRenderer::Get().NewString();
        ids_companions_label_text[1] = TextRenderer::Get().NewString();
        ids_companions_label_text[2] = TextRenderer::Get().NewString();
        ids_companions_label_text[3] = TextRenderer::Get().NewString();
        ids_companions_stamina_back[0] = Renderer2DSolidColors::Get().NewRectangle();
        ids_companions_stamina_back[1] = Renderer2DSolidColors::Get().NewRectangle();
        ids_companions_stamina_back[2] = Renderer2DSolidColors::Get().NewRectangle();
        ids_companions_stamina_back[3] = Renderer2DSolidColors::Get().NewRectangle();
        ids_companions_stamina_filled[0] = Renderer2DImages::Get().NewImage();
        ids_companions_stamina_filled[1] = Renderer2DImages::Get().NewImage();
        ids_companions_stamina_filled[2] = Renderer2DImages::Get().NewImage();
        ids_companions_stamina_filled[3] = Renderer2DImages::Get().NewImage();
    }

    void
    CompanionsPanel::Update()
    /*/////////////////////*/
    {
        GuiMovableContainer::Update();
    }

    void
    CompanionsPanel::Render() const
    /*///////////////////////////*/
    {
        Renderer2DImages::Get().DrawImage(Hash("PanelBg"), id_background_image, GetBounds());
        TextRenderer::Get().DrawString(
            id_title_text, k_title_text.data(), GetPosition().Translate(0.014f, 0.018f));
        Renderer2DImages::Get().DrawImage(
            Hash("HorizontalSplitter"), id_horizntal_splitter,
            rel_bounds_horizontal_splitter.Translate(GetPosition().x, GetPosition().y));
        auto player_world_area_pos = Player::Get().GetWorldAreaPos();
        auto map_area = World::Get().GetMapAreaAtZLevel(player_world_area_pos.z);
        auto it = map_area->GetCompanionsMirror().cbegin();
        for (auto i = 0; i < 4; i++)
        /**************************/
        {
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
            TextRenderer::Get().DrawString(
                ids_companions_label_text[i], "Comp. " + std::to_string(i),
                GetPosition().Translate(0.01f, 0.07f + i * 0.04f));
            Renderer2DSolidColors::Get().FillRectangle(
                ids_companions_stamina_back[i], rectangle_back, Colors::black);
            Renderer2DImages::Get().DrawImage(
                "Yellow", ids_companions_stamina_filled[i], rectangle_filled);
            ++it;
        }
        GuiMovableContainer::Render();
    }
}
//////////////////////////////////////////////////////////////////////