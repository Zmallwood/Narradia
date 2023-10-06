//////////////////////////////////////////////////////////////////////
#include "ObjectBrowserGui.hpp"
#include "Engine/Assets.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "Scenes/EditorScene/Modules/ToolUsing.hpp"
#include "World/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectBrowserGui::ObjectBrowserGui()
        : GuiWindow("Objects browser", {0.3f, 0.2f, 0.2f, 0.45f})
    /*/////////////////////////////////////////////////////////*/
    {
        auto slotWidth = (GetBounds().width - 2 * GetMargin()) / numCols - GetMargin();
        auto images = ImageBank::Get().GetImages();
        int j = 0;
        for (auto &entry : images)
        /************************/
        {
            if (entry.second.fileName.substr(0, 6) == "Object")
            /*************************************************/
            {
                objectsLibrary.insert({j, std::make_shared<Object>(entry.second.fileName)});
                j++;
            }
        }
        auto i = 0;
        for (auto y = 0; y < numRows; y++)
        /********************************/
        {
            for (auto x = 0; x < numCols; x++)
            /********************************/
            {
                slots[x][y] =
                    std::make_shared<GuiWindowObjectSlot>(this, x, y, i, slotWidth, objectsLibrary);
                i++;
            }
        }
        bottomBarRendId = Renderer2DImages::Get().NewImage();
        AddGuiButton(
            "", {0.02f, 0.42f, 0.015f, ConvertWidthToHeight(0.015f)}, [&] { page--; },
            "GuiLeftArrow", "GuiLeftArrow");
        AddGuiButton(
            "", {0.15f, 0.42f, 0.015f, ConvertWidthToHeight(0.015f)}, [&] { page++; },
            "GuiRightArrow", "GuiRightArrow");
        idSelectedSlotFrame = Renderer2DImages::Get().NewImage();
    }

    void ObjectBrowserGui::UpdateDerived()
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
        for (auto y = 0; y < numRows; y++)
        /********************************/
        {
            for (auto x = 0; x < numCols; x++)
            /********************************/
            {
                slots[x][y]->Update(page * numRows * numCols);
                if (slots[x][y]->GetBounds().Contains(GetMousePositionF()))
                /*********************************************************/
                {
                    MouseInput::Get().GetLeftButton().AddFiredAction(
                        "SelectObject", [&]
                        /*****************/
                        {
                            if (GuiWindowObjectSlot::hoveredObject)
                            /*************************************/
                            {
                                selectedObjectIndex = GuiWindowObjectSlot::hoveredIndex;
                                ToolUsing::Get().ChangeTool(Tools::AddObject);
                                ToolUsing::Get().SelectType(
                                    GuiWindowObjectSlot::hoveredObject->GetObjectType());
                            }
                        });
                }
            }
        }
    }

    void ObjectBrowserGui::RenderDerived() const
    /*////////////////////////////////////////*/
    {
        auto i = 0;
        for (auto y = 0; y < numRows; y++)
        /********************************/
        {
            for (auto x = 0; x < numCols; x++)
            /********************************/
            {
                slots[x][y]->Render(page * numRows * numCols);
                if (i + page * numRows * numCols == selectedObjectIndex)
                /******************************************************/
                {
                    Renderer2DImages::Get().DrawImage(
                        "GuiSelectedSlotFrame", idSelectedSlotFrame, slots[x][y]->GetBounds());
                }
                i++;
            }
        }
        auto bottomBarHeight = 0.04f;
        auto bottomBarBounds = RectangleF{
            GetBounds().x, GetBounds().y + GetBounds().height - bottomBarHeight, GetBounds().width,
            bottomBarHeight};
        Renderer2DImages::Get().DrawImage(
            "GuiWindowInvBottomBarBg", bottomBarRendId, bottomBarBounds);
    }
}
//////////////////////////////////////////////////////////////////////