//////////////////////////////////////////////////////////////////////
#include "OpenContainerGui.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    OpenContainerGui::OpenContainerGui(Object *object_)
        : GuiWindow("Container", {0.15f, 0.3f, 0.17f, 0.35f}, true),
          object(object_)
    /*////////////////////////////////////////////////////////////*/
    {
        Show();
        auto slotWidth = (GetBounds().width - 2 * GetMargin()) / maxNumCols - GetMargin();
        numSlots = ObjectBehaviourList::Get().GetContainerSlots(object_->GetObjectType());
        auto i = 0;
        for (auto y = 0; y < maxNumRows; y++)
        /***********************************/
        {
            for (auto x = 0; x < maxNumCols; x++)
            /***********************************/
            {
                if (i >= numSlots)
                    break;
                slots[x][y] = std::make_shared<GuiWindowObjectSlot>(
                    this, x, y, i, slotWidth, object_->GetContainedObjects());
                i++;
            }
        }
    }

    void OpenContainerGui::UpdateDerived()
    /*//////////////////////////////////*/
    {
        if (GetBounds().Contains(GetMousePositionF()))
        /********************************************/
        {
            GuiWindowObjectSlot::hoveredObject = nullptr;
            GuiWindowObjectSlot::hoveredIndex = -1;
            GuiWindowObjectSlot::activeWindow = this;
        }
        if (GuiWindowObjectSlot::activeWindow == this)
        /********************************************/
        {
            GuiWindowObjectSlot::hoveredObject = nullptr;
            GuiWindowObjectSlot::hoveredIndex = -1;
        }
        auto i = 0;
        for (auto y = 0; y < maxNumRows; y++)
        /***********************************/
        {
            for (auto x = 0; x < maxNumCols; x++)
            /***********************************/
            {
                if (i >= numSlots)
                    break;
                slots[x][y]->Update();
                i++;
            }
        }
    }

    void OpenContainerGui::RenderDerived() const
    /*////////////////////////////////////////*/
    {
        auto i = 0;
        for (auto y = 0; y < maxNumRows; y++)
        /***********************************/
        {
            for (auto x = 0; x < maxNumCols; x++)
            /***********************************/
            {
                if (i >= numSlots)
                    break;
                slots[x][y]->Render();
                i++;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////