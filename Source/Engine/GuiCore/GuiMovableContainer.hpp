#pragma once
#include "GuiContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiMovableContainer : public GuiContainer {
      public:
        GuiMovableContainer(RectangleF, Point2F, SizeF);

      protected:
        void Update() override;
        void Render() const override;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}