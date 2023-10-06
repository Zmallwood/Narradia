//////////////////////////////////////////////////////////////////////
#include "FpsPanel.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class FpsPanel::Pimpl
    /*/////////////////*/
    {
      public:
        static constexpr Point2F kStartPosition = {0.9f, 0.01f};
        static constexpr SizeF kSize = {0.08f, 0.04f};
        int fps = 0;
        int framesCount = 0;
        int ticksLastUpdate = 0;
        RenderId idBackgroundImage;
        RenderId idFpsText;
        bool visible = false;
    };

    FpsPanel::FpsPanel()
        : p(std::make_shared<Pimpl>()),
          GuiMovableContainer(Pimpl::kSize.ToRectangle(), Pimpl::kStartPosition, Pimpl::kSize)
    /*//////////////////////////////////////////////////////////////////////////////////////*/
    {
        p->idBackgroundImage = Renderer2DImages::Get().NewImage();
        p->idFpsText = TextRenderer::Get().NewString();
    }

    void
    FpsPanel::Update()
    /*///////////////////*/
    {
        if (SDL_GetTicks() > p->ticksLastUpdate + 1000)
        /*********************************************/
        {
            p->fps = p->framesCount++;
            p->framesCount = 0;
            p->ticksLastUpdate = SDL_GetTicks();
        }
        p->framesCount++;
        GuiMovableContainer::Update();
    }

    void
    FpsPanel::Render() const
    /*/////////////////////////*/
    {
        if (!p->visible)
            return;
        Renderer2DImages::Get().DrawImage(Hash("PanelBg"), p->idBackgroundImage, GetBounds());
        std::string_view fpsText = "Fps: " + std::to_string(p->fps);
        TextRenderer::Get().DrawString(
            p->idFpsText, fpsText, GetPosition().Translate(0.014f, 0.018f));
        GuiMovableContainer::Render();
    }
}
//////////////////////////////////////////////////////////////////////