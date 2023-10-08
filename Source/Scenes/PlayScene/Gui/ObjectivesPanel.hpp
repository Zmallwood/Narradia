#pragma once
#include "Engine/GuiCore/GuiMovableContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectivesPanel : public GuiMovableContainer, public Singleton<ObjectivesPanel> {
      public:
        ObjectivesPanel();

      private:
        void Update() override;
        void Render() const override;

        static constexpr Point2F kStartPosition = {0.8f, 0.3f};
        static constexpr SizeF kSize = {0.18f, 0.17f};
        static constexpr std::string_view kTitleText = "Objectives";
        const RectangleF kRelBoundsHorizontalSplitter;
        RenderId rendid_background_image_;
        RenderId rendid_horizontal_splitter_;
        RenderId rendid_title_text_;
        RenderId rendid_progress_text_;
        RenderId rendid_final_objective_text_;
    };
}