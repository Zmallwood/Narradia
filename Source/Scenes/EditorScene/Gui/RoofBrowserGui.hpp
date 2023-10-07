#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindowObjectSlot;
    class Object;
    
    class RoofBrowserGui : public GuiWindow, public Singleton<RoofBrowserGui>
    /*/////////////////////////////////////////////////////////////////////*/
    {
      public:
        RoofBrowserGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
        int selectedObjectIndex = -1;
        int page = 0;
        std::shared_ptr<GuiWindowObjectSlot> slots[5][6];
        const int numRows = 6;
        const int numCols = 5;
        RenderId bottomBarRendId;
        std::map<int, std::shared_ptr<Object>> objectsLibrary;
        RenderId idSelectedSlotFrame;
    };
}