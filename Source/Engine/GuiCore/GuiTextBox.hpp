//////////////////////////////////////////////////////////////////////
#pragma once
#include "GuiComponent.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiContainer;

    class GuiTextBox : public GuiComponent
    /*//////////////////////////////////*/
    {
      public:
        GuiTextBox(
            RectangleF, GuiContainer *, std::string = "",
            std::shared_ptr<std::function<void()>> = nullptr);
        void Update() override;
        void Render() const override;
        std::string_view GetText();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
        void SetText(const std::string &);
        friend class SplitStackGui;
    };
}
//////////////////////////////////////////////////////////////////////