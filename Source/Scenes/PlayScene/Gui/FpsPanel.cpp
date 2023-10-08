#include "FpsPanel.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class FpsPanel::Pimpl {
      public:
        static constexpr Point2F kStartPosition = {0.9f, 0.01f};
        static constexpr SizeF kSize = {0.08f, 0.04f};
        int fps_ = 0;
        int frames_count_ = 0;
        int ticks_last_update_ = 0;
        RenderId rendid_background_image_;
        RenderId rendid_fps_text_;
        bool visible_ = false;
    };

    FpsPanel::FpsPanel()
        : p(std::make_shared<Pimpl>()),
          GuiMovableContainer(Pimpl::kSize.ToRectangle(), Pimpl::kStartPosition, Pimpl::kSize) {
        p->rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_fps_text_ = TextRenderer::Get()->NewString();
    }

    void FpsPanel::Update() {
        if (SDL_GetTicks() > p->ticks_last_update_ + 1000) {
            p->fps_ = p->frames_count_++;
            p->frames_count_ = 0;
            p->ticks_last_update_ = SDL_GetTicks();
        }
        p->frames_count_++;
        GuiMovableContainer::Update();
    }

    void FpsPanel::Render() const {
        if (!p->visible_)
            return;
        Renderer2DImages::Get()->DrawImage(
            Hash("PanelBg"), p->rendid_background_image_, GetBounds());
        std::string_view fps_text = "Fps: " + std::to_string(p->fps_);
        TextRenderer::Get()->DrawString(
            p->rendid_fps_text_, fps_text, GetPosition().Translate(0.014f, 0.018f));
        GuiMovableContainer::Render();
    }
}