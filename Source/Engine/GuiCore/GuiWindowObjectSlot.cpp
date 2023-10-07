//////////////////////////////////////////////////////////////////////
#include "GuiWindowObjectSlot.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "GuiWindow.hpp"
#include "World/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    GuiWindowObjectSlot::GuiWindowObjectSlot(
        GuiWindow *parentWindow_, int x_, int y_, int i_, float slotWidth_,
        const std::map<int, std::shared_ptr<Object>> &objectsList_)
        : parentWindow(parentWindow_),
          x(x_),
          y(y_),
          i(i_),
          slotWidth(slotWidth_),
          objectsList(objectsList_)
    /*///////////////////////////////////////////////////////////////////*/
    {
        rendIdFrame = Renderer2DImages::Get()->NewImage();
        rendIdObject = Renderer2DImages::Get()->NewImage();
        rendIdQtyText = TextRenderer::Get()->NewString();
        rendIdTransformProgressBack = Renderer2DImages::Get()->NewImage();
        rendIdTransformProgressFilled = Renderer2DImages::Get()->NewImage();
    }

    void
    GuiWindowObjectSlot::Update(int offset) const
    /*/////////////////////////////////////////*/
    {
        auto mousePosF = GetMousePositionF();
        if (GetBounds().Contains(mousePosF))
        /**********************************/
        {
            hoveredIndex = i + offset;
            if (objectsList.count(i + offset) > 0)
            /************************************/
            {
                auto object = objectsList.at(i + offset);
                hoveredObject = object.get();
                return;
            }
        }
    }

    void
    GuiWindowObjectSlot::Render(int offset) const
    /*/////////////////////////////////////////*/
    {
        Log();
        auto slotHeight = GetSlotHeight();
        Log();
        Renderer2DImages::Get()->DrawImage(frameImgNameHash, rendIdFrame, GetBounds());
        Log();
        Object *object = nullptr;
        Log();
        if (objectsList.count(i + offset) > 0)
        /************************************/
        {
            Log();
            object = objectsList.at(i + offset).get();
        }
        Log();
        if (object)
        /*********/
        {
            Log();
            auto imageNameHash = object->GetObjectType();
            Renderer2DImages::Get()->DrawImage(imageNameHash, rendIdObject, GetBounds());
            if (object->GetQuantity() > 1)
            /****************************/
            {
                Log();
                auto qtyText = "x" + std::to_string(object->GetQuantity());
                TextRenderer::Get()->DrawString(
                    rendIdQtyText, qtyText,
                    GetBounds().GetPosition().Translate(slotWidth * 0.65f, slotHeight * 0.7f));
            }
            if (object->GetTransformationProgress() > 0.0f)
            /*********************************************/
            {
                Log();
                auto pos =
                    GetBounds().GetPosition().Translate(slotWidth * 0.01f, slotHeight * 0.1f);
                auto height = slotHeight * 0.2f;
                auto width = slotWidth * 0.8f;
                auto widthFilled = width * object->GetTransformationProgress();
                auto rect = RectangleF{pos.x, pos.y, width, height};
                auto rectFilled = RectangleF{pos.x, pos.y, widthFilled, height};
                Renderer2DImages::Get()->DrawImage("Black", rendIdTransformProgressBack, rect);
                Renderer2DImages::Get()->DrawImage(
                    "Yellow", rendIdTransformProgressFilled, rectFilled);
            }
        }
    }

    RectangleF
    GuiWindowObjectSlot::GetBounds() const
    /*//////////////////////////////////*/
    {
        auto bounds = parentWindow->GetBounds();
        auto margin = parentWindow->GetMargin();
        auto slotHeight = GetSlotHeight();
        auto titleBarHeight = parentWindow->GetTitleBarHeight();
        float xPos = bounds.x + margin + x * (slotWidth + margin);
        float yPos = bounds.y + titleBarHeight + margin + y * (slotHeight + margin);
        float w = slotWidth;
        float h = slotHeight;
        return {xPos, yPos, w, h};
    }

    float
    GuiWindowObjectSlot::GetSlotHeight() const
    /*//////////////////////////////////////*/
    {
        return ConvertWidthToHeight(slotWidth);
    }
}