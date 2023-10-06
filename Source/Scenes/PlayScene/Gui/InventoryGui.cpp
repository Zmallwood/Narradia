//////////////////////////////////////////////////////////////////////
#include "InventoryGui.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    InventoryGui::InventoryGui()
        : GuiWindow("Inventory", {0.3f, 0.2f, 0.2f, 0.45f})
    /*///////////////////////////////////////////////////*/
    {
        auto slotWidth = (GetBounds().width - 2 * GetMargin()) / numCols - GetMargin();
        auto i = 0;
        for (auto y = 0; y < numRows; y++)
        /********************************/
        {
            for (auto x = 0; x < numCols; x++)
            /********************************/
            {
                slots[x][y] = std::make_shared<GuiWindowObjectSlot>(
                    this, x, y, i, slotWidth, Player::Get().data.inventory.objects);
                i++;
            }
        }
        bottomBarRendId = Renderer2DImages::Get().NewImage();
    }

    void
    InventoryGui::UpdateDerived()
    /*//////////////////////////////*/
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
        for (auto y = 0; y < numRows; y++)
            for (auto x = 0; x < numCols; x++)
                slots[x][y]->Update();
    }

    void
    InventoryGui::RenderDerived() const
    /*////////////////////////////////////*/
    {
        Log();
        for (auto y = 0; y < numRows; y++)
            for (auto x = 0; x < numCols; x++)
                slots[x][y]->Render();
        Log();
        auto bottomBarHeight = 0.04f;
        Log();
        auto bottomBarBounds = RectangleF{
            GetBounds().x, GetBounds().y + GetBounds().height - bottomBarHeight, GetBounds().width,
            bottomBarHeight};
        Log();
        Renderer2DImages::Get().DrawImage(
            "GuiWindowInvBottomBarBg", bottomBarRendId, bottomBarBounds);
    }
}
//////////////////////////////////////////////////////////////////////