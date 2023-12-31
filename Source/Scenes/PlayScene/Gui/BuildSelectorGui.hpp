#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class BuildSelectorGui : public GuiWindow, public Singleton<BuildSelectorGui> {
      public:
        BuildSelectorGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
    };
}