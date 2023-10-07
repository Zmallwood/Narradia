#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiComponent;

    class SceneGui
    /*/////////*/
    {
      public:
        SceneGui();
        void AddGuiButton(
            std::string_view, RectangleF, std::function<void()>, std::string_view = "",
            std::string_view = "");
        void Update();
        void Render() const;
        void AddGuiComponent(std::shared_ptr<GuiComponent>);
        void RemoveGuiComponent(GuiComponent *);
        auto GetGuiComponents() const -> const std::vector<std::shared_ptr<GuiComponent>> &;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}