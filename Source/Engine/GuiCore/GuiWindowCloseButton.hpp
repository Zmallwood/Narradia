//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindow;

    class GuiWindowCloseButton
    /*//////////////////////*/
    {
      public:
        GuiWindowCloseButton() = default;
        GuiWindowCloseButton(GuiWindow *);
        void Update();
        void Render() const;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}