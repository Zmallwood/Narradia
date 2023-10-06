//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindowObjectSlot;
    
    class ObjectBrowserGui : public GuiWindow, public Singleton<ObjectBrowserGui>
    /*/////////////////////////////////////////////////////////////////////////*/
    {
      public:
        ObjectBrowserGui();

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
//////////////////////////////////////////////////////////////////////