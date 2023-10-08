#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiObjectHovering : public Singleton<GuiObjectHovering>
    /*/////////////////////////////////////////////////////////*/
    {
      public:
        GuiObjectHovering();
        void Render();

      private:
        RenderId rendid_label_box_;
        RenderId rendid_label_text_;
    };
}