#pragma once
#include "GuiComponent.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiContainer : public GuiComponent {
      public:
        GuiContainer(Point2F, SizeF);
        RectangleF GetBounds() const;
        const Point2F &GetPosition() const;
        const SizeF &GetSize() const;

      protected:
        void Update() override;
        void Render() const override;
        void AddGuiButton(
            const std::string_view &, RectangleF, std::function<void()>,
            const std::string_view & = "", const std::string_view & = "");
        void AddGuiComponent(std::shared_ptr<GuiComponent>);
        void SetPosition(Point2F);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}