#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;
    class GuiWindow;

    class GuiWindowObjectSlot {
      public:
        GuiWindowObjectSlot(
            GuiWindow *, int, int, int, float, const std::map<int, std::shared_ptr<Object>> &);
        RectangleF GetBounds() const;
        inline static Object *hovered_object_ = nullptr;
        inline static int hovered_index_ = -.1;
        inline static GuiWindow *active_window_ = nullptr;

      private:
        void Update(int offset = 0) const;
        void Render(int offset = 0) const;
        float GetSlotHeight() const;
        const std::map<int, std::shared_ptr<Object>> &kObjectsList;
        const int kFrameImageNameHash = Hash("InvSlotBack");
        RenderId rendid_frame_;
        RenderId rendid_object_;
        RenderId rendid_quantity_text_;
        RenderId rendid_transform_progress_back_;
        RenderId rendid_transform_progress_filled_;
        int x_;
        int y_;
        int i_;
        GuiWindow *parent_window_ = nullptr;
        float slot_width_;
        
        friend class InventoryGui;
        friend class OpenContainerGui;
        friend class ObjectBrowserGui;
        friend class GroundBrowserGui;
        friend class RoofBrowserGui;
        friend class MobBrowserGui;
    };
}