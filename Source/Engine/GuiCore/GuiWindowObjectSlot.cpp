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
        : parent_window_(parentWindow_),
          x_(x_),
          y_(y_),
          i_(i_),
          slot_width_(slotWidth_),
          kObjectsList(objectsList_)
    /*///////////////////////////////////////////////////////////////////*/
    {
        rendid_frame_ = Renderer2DImages::Get()->NewImage();
        rendid_object_ = Renderer2DImages::Get()->NewImage();
        rendid_quantity_text_ = TextRenderer::Get()->NewString();
        rendid_transform_progress_back_ = Renderer2DImages::Get()->NewImage();
        rendid_transform_progress_filled_ = Renderer2DImages::Get()->NewImage();
    }

    void
    GuiWindowObjectSlot::Update(int offset) const
    /*/////////////////////////////////////////*/
    {
        auto mouse_position_f = GetMousePositionF();
        if (GetBounds().Contains(mouse_position_f))
        /**********************************/
        {
            hovered_index_ = i_ + offset;
            if (kObjectsList.count(i_ + offset) > 0)
            /************************************/
            {
                auto object = kObjectsList.at(i_ + offset);
                hovered_object_ = object.get();
                return;
            }
        }
    }

    void
    GuiWindowObjectSlot::Render(int offset) const
    /*/////////////////////////////////////////*/
    {
        Log();
        auto slot_height = GetSlotHeight();
        Log();
        Renderer2DImages::Get()->DrawImage(kFrameImageNameHash, rendid_frame_, GetBounds());
        Log();
        Object *object = nullptr;
        Log();
        if (kObjectsList.count(i_ + offset) > 0)
        /************************************/
        {
            Log();
            object = kObjectsList.at(i_ + offset).get();
        }
        Log();
        if (object)
        /*********/
        {
            Log();
            auto image_name_hash = object->GetObjectType();
            Renderer2DImages::Get()->DrawImage(image_name_hash, rendid_object_, GetBounds());
            if (object->GetQuantity() > 1)
            /****************************/
            {
                Log();
                auto quantity_text = "x" + std::to_string(object->GetQuantity());
                TextRenderer::Get()->DrawString(
                    rendid_quantity_text_, quantity_text,
                    GetBounds().GetPosition().Translate(slot_width_ * 0.65f, slot_height * 0.7f));
            }
            if (object->GetTransformationProgress() > 0.0f)
            /*********************************************/
            {
                Log();
                auto position =
                    GetBounds().GetPosition().Translate(slot_width_ * 0.01f, slot_height * 0.1f);
                auto height = slot_height * 0.2f;
                auto width = slot_width_ * 0.8f;
                auto width_filled = width * object->GetTransformationProgress();
                auto rect = RectangleF{position.x, position.y, width, height};
                auto rect_filled = RectangleF{position.x, position.y, width_filled, height};
                Renderer2DImages::Get()->DrawImage("Black", rendid_transform_progress_back_, rect);
                Renderer2DImages::Get()->DrawImage(
                    "Yellow", rendid_transform_progress_filled_, rect_filled);
            }
        }
    }

    RectangleF
    GuiWindowObjectSlot::GetBounds() const
    /*//////////////////////////////////*/
    {
        auto bounds = parent_window_->GetBounds();
        auto margin = parent_window_->GetMargin();
        auto slot_height = GetSlotHeight();
        auto title_bar_height = parent_window_->GetTitleBarHeight();
        float x_position = bounds.x + margin + x_ * (slot_width_ + margin);
        float y_position = bounds.y + title_bar_height + margin + y_ * (slot_height + margin);
        float w = slot_width_;
        float h = slot_height;
        return {x_position, y_position, w, h};
    }

    float
    GuiWindowObjectSlot::GetSlotHeight() const
    /*//////////////////////////////////////*/
    {
        return ConvertWidthToHeight(slot_width_);
    }
}