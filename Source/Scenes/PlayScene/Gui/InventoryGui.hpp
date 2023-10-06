//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindowObjectSlot;
    
    class InventoryGui : public GuiWindow, public Singleton<InventoryGui>
    /*/////////////////////////////////////////////////////////////////*/
    {
      public:
        InventoryGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
        std::shared_ptr<GuiWindowObjectSlot> slots[5][6];
        const int numRows = 6;
        const int numCols = 5;
        RenderId bottomBarRendId;
    };
}
//////////////////////////////////////////////////////////////////////