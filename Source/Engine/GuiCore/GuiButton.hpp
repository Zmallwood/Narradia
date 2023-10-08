#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiContainer;

    class GuiButton {
      public:
        GuiButton(
            const std::string_view &, RectangleF, std::function<void()>, GuiContainer * = nullptr,
            const std::string_view & = "GuiButtonBack",
            const std::string_view & = "GuiButtonBackHovered");
        void Update();
        void Render() const;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}