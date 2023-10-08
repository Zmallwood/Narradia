#include "GuiObjectHovering.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    GuiObjectHovering::GuiObjectHovering()
    /*//////////////////////////////////*/
    {
        rendid_label_box_ = Renderer2DSolidColors::Get()->NewRectangle();
        rendid_label_text_ = TextRenderer::Get()->NewString();
    }

    void
    GuiObjectHovering::Render()
    /*///////////////////////*/
    {
        if (GuiWindowObjectSlot::hovered_object_)
        /*************************************/
        {
            auto box_position = GetMousePositionF().Translate(0.003f, 0.01f);
            auto rect = RectangleF{box_position.x, box_position.y, 0.07f, 0.03f};
            Renderer2DSolidColors::Get()->FillRectangle(rendid_label_box_, rect, Colors::alphaBlack);
            std::string label_text;
            if (ObjectBehaviourList::Get()->HasBehaviourData(
                    GuiWindowObjectSlot::hovered_object_->GetObjectType()))
            /***********************************************************/
            {
                label_text = ObjectBehaviourList::Get()->GetLabel(
                    GuiWindowObjectSlot::hovered_object_->GetObjectType());
            }
            else
            /**/
            {
                auto images = ImageBank::Get()->GetImages();
                auto object_type = GuiWindowObjectSlot::hovered_object_->GetObjectType();
                if (images.count(object_type))
                /***************************/
                {
                    label_text = images.at(object_type).fileName;
                }
                else
                /**/
                {
                    label_text = "Unnamed";
                }
            }
            TextRenderer::Get()->DrawString(
                rendid_label_text_, label_text, box_position.Translate(0.004f, 0.012f), Colors::wheat);
        }
    }
}