#include "OpenContainerGui.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    OpenContainerGui::OpenContainerGui(Object *object_)
        : GuiWindow("Container", {0.15f, 0.3f, 0.17f, 0.35f}, true),
          object(object_) {
        Show();
        auto slot_width = (GetBounds().width - 2 * GetMargin()) / kMaxNumCols - GetMargin();
        num_slots_ = ObjectBehaviourList::Get()->GetContainerSlots(object_->GetObjectType());
        auto i = 0;
        for (auto y = 0; y < kMaxNumRows; y++) {
            for (auto x = 0; x < kMaxNumCols; x++) {
                if (i >= num_slots_)
                    break;
                slots_[x][y] = std::make_shared<GuiWindowObjectSlot>(
                    this, x, y, i, slot_width, object_->GetContainedObjects());
                i++;
            }
        }
    }

    void OpenContainerGui::UpdateDerived() {
        if (GetBounds().Contains(GetMousePositionF())) {
            GuiWindowObjectSlot::hovered_object_ = nullptr;
            GuiWindowObjectSlot::hovered_index_ = -1;
            GuiWindowObjectSlot::active_window_ = this;
        }
        if (GuiWindowObjectSlot::active_window_ == this) {
            GuiWindowObjectSlot::hovered_object_ = nullptr;
            GuiWindowObjectSlot::hovered_index_ = -1;
        }
        auto i = 0;
        for (auto y = 0; y < kMaxNumRows; y++) {
            for (auto x = 0; x < kMaxNumCols; x++) {
                if (i >= num_slots_)
                    break;
                slots_[x][y]->Update();
                i++;
            }
        }
    }

    void OpenContainerGui::RenderDerived() const {
        auto i = 0;
        for (auto y = 0; y < kMaxNumRows; y++) {
            for (auto x = 0; x < kMaxNumCols; x++) {
                if (i >= num_slots_)
                    break;
                slots_[x][y]->Render();
                i++;
            }
        }
    }
}