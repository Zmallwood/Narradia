#include "GroundBrowserGui.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "Scenes/EditorScene/Modules/ToolUsing.hpp"
#include "World/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    GroundBrowserGui::GroundBrowserGui()
        : GuiWindow("Ground browser", {0.5f, 0.2f, 0.2f, 0.45f}) {
        auto slot_width = (GetBounds().width - 2 * GetMargin()) / kNumCols - GetMargin();
        auto images = ImageBank::Get()->GetImages();
        int j = 0;
        for (auto &entry : images) {
            if (entry.second.fileName.substr(0, 6) == "Ground") {
                objects_library_.insert({j, std::make_shared<Object>(entry.second.fileName)});
                j++;
            }
        }
        auto i = 0;
        for (auto y = 0; y < kNumRows; y++) {
            for (auto x = 0; x < kNumCols; x++) {
                slots_[x][y] = std::make_shared<GuiWindowObjectSlot>(
                    this, x, y, i, slot_width, objects_library_);
                i++;
            }
        }
        rendid_bottom_bar_ = Renderer2DImages::Get()->NewImage();
        AddGuiButton(
            "", {0.02f, 0.42f, 0.015f, ConvertWidthToHeight(0.015f)}, [&] { page_--; },
            "GuiLeftArrow", "GuiLeftArrow");
        AddGuiButton(
            "", {0.15f, 0.42f, 0.015f, ConvertWidthToHeight(0.015f)}, [&] { page_++; },
            "GuiRightArrow", "GuiRightArrow");
        rendid_selected_slot_frame_ = Renderer2DImages::Get()->NewImage();
    }

    void GroundBrowserGui::UpdateDerived() {
        if (GetBounds().Contains(GetMousePositionF())) {
            GuiWindowObjectSlot::hovered_object_ = nullptr;
            GuiWindowObjectSlot::hovered_index_ = -1;
            GuiWindowObjectSlot::active_window_ = this;
        }
        if (GuiWindowObjectSlot::active_window_ == this) {
            GuiWindowObjectSlot::hovered_object_ = nullptr;
            GuiWindowObjectSlot::hovered_index_ = -1;
        }
        for (auto y = 0; y < kNumRows; y++) {
            for (auto x = 0; x < kNumCols; x++) {
                slots_[x][y]->Update(page_ * kNumRows * kNumCols);
                if (slots_[x][y]->GetBounds().Contains(GetMousePositionF())) {
                    MouseInput::Get()->GetLeftButton().AddFiredAction("SelectObject", [&] {
                        if (GuiWindowObjectSlot::hovered_object_) {
                            selected_object_index_ = GuiWindowObjectSlot::hovered_index_;
                            ToolUsing::Get()->ChangeTool(Tools::SetGround);
                            ToolUsing::Get()->SelectType(
                                GuiWindowObjectSlot::hovered_object_->GetObjectType());
                        }
                    });
                }
            }
        }
    }

    void GroundBrowserGui::RenderDerived() const {
        auto i = 0;
        for (auto y = 0; y < kNumRows; y++) {
            for (auto x = 0; x < kNumCols; x++) {
                slots_[x][y]->Render(page_ * kNumRows * kNumCols);
                if (i + page_ * kNumRows * kNumCols == selected_object_index_) {
                    Renderer2DImages::Get()->DrawImage(
                        "GuiSelectedSlotFrame", rendid_selected_slot_frame_,
                        slots_[x][y]->GetBounds());
                }
                i++;
            }
        }
        auto bottom_bar_height = 0.04f;
        auto bottom_bar_bounds = RectangleF{
            GetBounds().x, GetBounds().y + GetBounds().height - bottom_bar_height,
            GetBounds().width, bottom_bar_height};
        Renderer2DImages::Get()->DrawImage(
            "GuiWindowInvBottomBarBg", rendid_bottom_bar_, bottom_bar_bounds);
    }
}