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
        RenderId idLabelBox;
        RenderId idLabelText;
    };
}