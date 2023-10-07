#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;
    class GuiWindow;

    class GuiWindowObjectSlot
    /*/////////////////////*/
    {
      public:
        GuiWindowObjectSlot(
            GuiWindow *, int, int, int, float, const std::map<int, std::shared_ptr<Object>> &);
        RectangleF GetBounds() const;
        inline static Object *hoveredObject = nullptr;
        inline static int hoveredIndex = -.1;
        inline static GuiWindow *activeWindow = nullptr;

      private:
        void Update(int offset = 0) const;
        void Render(int offset = 0) const;
        float GetSlotHeight() const;
        const std::map<int, std::shared_ptr<Object>> &objectsList;
        const int frameImgNameHash = Hash("InvSlotBack");
        RenderId rendIdFrame;
        RenderId rendIdObject;
        RenderId rendIdQtyText;
        RenderId rendIdTransformProgressBack;
        RenderId rendIdTransformProgressFilled;
        int x;
        int y;
        int i;
        GuiWindow *parentWindow = nullptr;
        float slotWidth;
        friend class InventoryGui;
        friend class OpenContainerGui;
        friend class ObjectBrowserGui;
        friend class GroundBrowserGui;
        friend class RoofBrowserGui;
        friend class MobBrowserGui;
    };
}