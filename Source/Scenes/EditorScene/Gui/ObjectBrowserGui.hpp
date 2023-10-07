#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindowObjectSlot;
    class Object;
    
    class ObjectBrowserGui : public GuiWindow, public Singleton<ObjectBrowserGui>
    /*/////////////////////////////////////////////////////////////////////////*/
    {
      public:
        ObjectBrowserGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;

        const int kNumRows = 6;
        const int kNumCols = 5;
        int selected_object_index_ = -1;
        int page_ = 0;
        std::shared_ptr<GuiWindowObjectSlot> slots_[5][6];
        RenderId rendid_bottom_bar_;
        std::map<int, std::shared_ptr<Object>> objects_library_;
        RenderId rendid_selected_slot_frame_;
    };
}