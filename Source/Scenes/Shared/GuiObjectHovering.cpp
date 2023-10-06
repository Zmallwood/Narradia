//////////////////////////////////////////////////////////////////////
#include "GuiObjectHovering.hpp"
#include "Engine/Assets.hpp"
#include "Engine/GuiCore.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    GuiObjectHovering::GuiObjectHovering()
    /*//////////////////////////////////*/
    {
        idLabelBox = Renderer2DSolidColors::Get().NewRectangle();
        idLabelText = TextRenderer::Get().NewString();
    }

    void GuiObjectHovering::Render()
    /*////////////////////////////*/
    {
        if (GuiWindowObjectSlot::hoveredObject)
        /*************************************/
        {
            auto boxPos = GetMousePositionF().Translate(0.003f, 0.01f);
            auto rect = RectangleF{boxPos.x, boxPos.y, 0.07f, 0.03f};
            Renderer2DSolidColors::Get().FillRectangle(idLabelBox, rect, Colors::alphaBlack);
            std::string labelText;
            if (ObjectBehaviourList::Get().HasBehaviourData(
                    GuiWindowObjectSlot::hoveredObject->GetObjectType()))
            /***********************************************************/
            {
                labelText = ObjectBehaviourList::Get().GetLabel(
                    GuiWindowObjectSlot::hoveredObject->GetObjectType());
            }
            else
            /**/
            {
                auto images = ImageBank::Get().GetImages();
                auto objectType = GuiWindowObjectSlot::hoveredObject->GetObjectType();
                if (images.count(objectType))
                /***************************/
                {
                    labelText = images.at(objectType).fileName;
                }
                else
                /**/
                {
                    labelText = "Unnamed";
                }
            }
            TextRenderer::Get().DrawString(
                idLabelText, labelText, boxPos.Translate(0.004f, 0.012f), Colors::wheat);
        }
    }
}
//////////////////////////////////////////////////////////////////////