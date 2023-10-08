#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;
    class GuiWindowObjectSlot;

    class OpenContainerGui : public GuiWindow {
      public:
        OpenContainerGui(Object *);

        Object *object;

      protected:
        void UpdateDerived() override;
        void RenderDerived() const override;

      private:
        static constexpr int kMaxNumRows = 6;
        static constexpr int kMaxNumCols = 5;
        int num_slots_;
        std::shared_ptr<GuiWindowObjectSlot> slots_[kMaxNumCols][kMaxNumRows];
    };
}