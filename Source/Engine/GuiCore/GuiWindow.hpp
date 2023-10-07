#pragma once
#include "GuiMovableContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindow : public GuiMovableContainer
    /*////////////////////////////////////////*/
    {
      public:
        GuiWindow(
            std::string_view title_, RectangleF bounds_, bool destroyOnClose_ = false,
            std::string_view backgroundImageName_ = "PanelBg");
        void ToggleVisibility();
        std::string_view GetTitle();
        bool DestroyOnClose();
        void Hide();
        float GetMargin();
        float GetTitleBarHeight();

      protected:
        virtual void UpdateDerived() = 0;
        virtual void RenderDerived() const = 0;
        void Show();

      private:
        void Update() override;
        void Render() const override;
        RectangleF GetAbsoluteTitleBarBounds() const;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}