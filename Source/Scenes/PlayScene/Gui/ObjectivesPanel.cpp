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
          relBoundsHorizontalSplitter({0.005f, 0.035f, kSize.width - 2 * 0.005f, 0.0015f})
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        glIdBackgroundImage = Renderer2DImages::Get()->NewImage();
        glIdHorizontalSplitter = Renderer2DImages::Get()->NewImage();
        glIdTitleText = TextRenderer::Get()->NewString();
        glIdProgressText = TextRenderer::Get()->NewString();
        glIdFinalObjectiveText =
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
        Renderer2DImages::Get()->DrawImage(Hash("PanelBg"), glIdBackgroundImage, GetBounds());
        TextRenderer::Get()->DrawString(
            glIdTitleText, kTitleText.data(), GetPosition().Translate(0.014f, 0.018f));
        Renderer2DImages::Get()->DrawImage(
            Hash("HorizontalSplitter"), glIdHorizontalSplitter,
            relBoundsHorizontalSplitter.Translate(GetPosition().x, GetPosition().y));
        auto numClaimedTiles = Player::Get()->claimedTiles.size();
        auto totNumberOfTiles = MapArea::GetMapSize().width * MapArea::GetMapSize().height;
        auto progress = (int)(((float)numClaimedTiles) / totNumberOfTiles * 100);
        TextRenderer::Get()->DrawString(
            glIdProgressText,
            "Progress: " + std::to_string(numClaimedTiles) + "/" +
                std::to_string(totNumberOfTiles) + " claimed (" + std::to_string(progress) + "%)",
            GetPosition().Translate(0.014f, 0.07f));
        TextRenderer::Get()->DrawMultiLineString(
            glIdFinalObjectiveText,
            "Final objective: Gain control over the map by owning 51% of "
            "it.",
            GetPosition().Translate(0.014f, 0.12f));
        GuiMovableContainer::Render();
    }
}