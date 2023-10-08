#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindowObjectSlot;

    class InventoryGui : public GuiWindow, public Singleton<InventoryGui> {
      public:
        InventoryGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;

        const int kNumRows = 6;
        const int kNumCols = 5;
        std::shared_ptr<GuiWindowObjectSlot> slots_[5][6];
        RenderId rendid_bottom_bar_;
    };
}