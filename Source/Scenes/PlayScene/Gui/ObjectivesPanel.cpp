#include "ObjectivesPanel.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectivesPanel::ObjectivesPanel()
        : GuiMovableContainer(kSize.ToRectangle(), kStartPosition, kSize),
          kRelBoundsHorizontalSplitter({0.005f, 0.035f, kSize.width - 2 * 0.005f, 0.0015f})
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        rendid_horizontal_splitter_ = Renderer2DImages::Get()->NewImage();
        rendid_title_text_ = TextRenderer::Get()->NewString();
        rendid_progress_text_ = TextRenderer::Get()->NewString();
        rendid_final_objective_text_ =
            TextRenderer::Get()->NewMultiLineString(5, kSize.width - 2 * 0.014f);
    }

    void
    ObjectivesPanel::Update()
    /*/////////////////////*/
    {
        GuiMovableContainer::Update();
    }

    void
    ObjectivesPanel::Render() const
    /*///////////////////////////*/
    {
        Renderer2DImages::Get()->DrawImage(Hash("PanelBg"), rendid_background_image_, GetBounds());
        TextRenderer::Get()->DrawString(
            rendid_title_text_, kTitleText.data(), GetPosition().Translate(0.014f, 0.018f));
        Renderer2DImages::Get()->DrawImage(
            Hash("HorizontalSplitter"), rendid_horizontal_splitter_,
            kRelBoundsHorizontalSplitter.Translate(GetPosition().x, GetPosition().y));
        auto numClaimedTiles = Player::Get()->claimedTiles.size();
        auto totNumberOfTiles = MapArea::GetMapSize().width * MapArea::GetMapSize().height;
        auto progress = (int)(((float)numClaimedTiles) / totNumberOfTiles * 100);
        TextRenderer::Get()->DrawString(
            rendid_progress_text_,
            "Progress: " + std::to_string(numClaimedTiles) + "/" +
                std::to_string(totNumberOfTiles) + " claimed (" + std::to_string(progress) + "%)",
            GetPosition().Translate(0.014f, 0.07f));
        TextRenderer::Get()->DrawMultiLineString(
            rendid_final_objective_text_,
            "Final objective: Gain control over the map by owning 51% of "
            "it.",
            GetPosition().Translate(0.014f, 0.12f));
        GuiMovableContainer::Render();
    }
}