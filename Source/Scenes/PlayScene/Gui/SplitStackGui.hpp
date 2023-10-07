//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    class SplitStackGui : public GuiWindow
    /*//////////////////////////////////*/
    {
      public:
        SplitStackGui(Object *);

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
        Object *object;
    };
}