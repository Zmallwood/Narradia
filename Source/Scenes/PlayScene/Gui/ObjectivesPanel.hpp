//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore/GuiMovableContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectivesPanel : public GuiMovableContainer, public Singleton<ObjectivesPanel>
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
      public:
        ObjectivesPanel();

      private:
        void Update() override;
        void Render() const override;
        RenderId glIdBackgroundImage;
        RenderId glIdHorizontalSplitter;
        RenderId glIdTitleText;
        RenderId glIdProgressText;
        RenderId glIdFinalObjectiveText;
        static constexpr Point2F kStartPosition = {0.8f, 0.3f};
        static constexpr SizeF kSize = {0.18f, 0.17f};
        static constexpr std::string_view kTitleText = "Objectives";
        const RectangleF relBoundsHorizontalSplitter;
    };
}
//////////////////////////////////////////////////////////////////////