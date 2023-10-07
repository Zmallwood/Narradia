#pragma once
#include "Engine/GuiCore/GuiMovableContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class CompanionsPanel : public GuiMovableContainer, public Singleton<CompanionsPanel>
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
      public:
        CompanionsPanel();

      private:
        void Update() override;
        void Render() const override;
        static constexpr Point2F kStartPosition = {0.02f, 0.2f};
        static constexpr SizeF kSize = {0.15f, 0.3f};
        static constexpr std::string_view kTitleText = "Companions";
        const RectangleF kRelBoundsHorizontalSplitter;
        RenderId rendid_background_image_;
        RenderId rendid_horizontal_splitter_;
        RenderId rendid_title_text_;
        std::array<RenderId, 4> rendids_companions_label_texts_;
        std::array<RenderId, 4> rendids_companions_stamina_backs_;
        std::array<RenderId, 4> rendids_companions_stamina_filleds_;
    };
}