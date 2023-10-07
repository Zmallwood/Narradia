//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;
    class GuiWindowObjectSlot;
    
    class OpenContainerGui : public GuiWindow
    /*/////////////////////////////////////*/
    {
      public:
        OpenContainerGui(Object *);
        Object *object;

      protected:
        void UpdateDerived() override;
        void RenderDerived() const override;

      private:
        static constexpr int maxNumRows = 6;
        static constexpr int maxNumCols = 5;
        int numSlots;
        std::shared_ptr<GuiWindowObjectSlot> slots[maxNumCols][maxNumRows];
    };
}