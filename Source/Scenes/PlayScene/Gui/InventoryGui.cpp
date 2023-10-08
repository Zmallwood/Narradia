#include "InventoryGui.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    InventoryGui::InventoryGui()
        : GuiWindow("Inventory", {0.3f, 0.2f, 0.2f, 0.45f}) {
        auto slot_width = (GetBounds().width - 2 * GetMargin()) / kNumCols - GetMargin();
        auto i = 0;
        for (auto y = 0; y < kNumRows; y++) {
            for (auto x = 0; x < kNumCols; x++) {
                slots_[x][y] = std::make_shared<GuiWindowObjectSlot>(
                    this, x, y, i, slot_width, Player::Get()->data_.inventory_.objects_);
                i++;
            }
        }
        rendid_bottom_bar_ = Renderer2DImages::Get()->NewImage();
    }

    void InventoryGui::UpdateDerived() {
        if (GetBounds().Contains(GetMousePositionF())) {
            GuiWindowObjectSlot::hovered_object_ = nullptr;
            GuiWindowObjectSlot::hovered_index_ = -1;
            GuiWindowObjectSlot::active_window_ = this;
        }
        if (GuiWindowObjectSlot::active_window_ == this) {
            GuiWindowObjectSlot::hovered_object_ = nullptr;
            GuiWindowObjectSlot::hovered_index_ = -1;
        }
        for (auto y = 0; y < kNumRows; y++)
            for (auto x = 0; x < kNumCols; x++)
                slots_[x][y]->Update();
    }

    void InventoryGui::RenderDerived() const {
        Log();
        for (auto y = 0; y < kNumRows; y++)
            for (auto x = 0; x < kNumCols; x++)
                slots_[x][y]->Render();
        Log();
        auto bottom_bar_height = 0.04f;
        Log();
        auto bottom_bar_bounds = RectangleF{
            GetBounds().x, GetBounds().y + GetBounds().height - bottom_bar_height,
            GetBounds().width, bottom_bar_height};
        Log();
        Renderer2DImages::Get()->DrawImage(
            "GuiWindowInvBottomBarBg", rendid_bottom_bar_, bottom_bar_bounds);
    }
}