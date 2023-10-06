//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiComponent
    /*//////////////*/
    {
      public:
        virtual void Update() = 0;
        virtual void Render() const = 0;
    };

    class GuiContainer : public GuiComponent
    /*////////////////////////////////////*/
    {
      public:
        GuiContainer(Point2F, SizeF);
        RectangleF GetBounds() const;
        const Point2F &GetPosition() const;
        const SizeF &GetSize() const;

      protected:
        void Update() override;
        void Render() const override;
        void AddGuiButton(
            const std::string_view &, RectangleF, std::function<void()>,
            const std::string_view & = "", const std::string_view & = "");
        void AddGuiComponent(std::shared_ptr<GuiComponent>);
        void SetPosition(Point2F);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class GuiMovableContainer : public GuiContainer
    /*///////////////////////////////////////////*/
    {
      public:
        GuiMovableContainer(RectangleF, Point2F, SizeF);

      protected:
        void Update() override;
        void Render() const override;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class GuiButton
    /*///////////*/
    {
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

    class GuiWindow : public GuiMovableContainer
    /*////////////////////////////////////////*/
    {
      public:
        GuiWindow(
            std::string_view title_, RectangleF bounds_, bool destroyOnClose_ = false,
            std::string_view backgroundImageName_ = "PanelBg");
        void ToggleVisibility();
        std::string_view GetTitle();
        bool DestroyOnClose();
        void Hide();
        float GetMargin();
        float GetTitleBarHeight();

      protected:
        virtual void UpdateDerived() = 0;
        virtual void RenderDerived() const = 0;
        void Show();

      private:
        void Update() override;
        void Render() const override;
        RectangleF GetAbsoluteTitleBarBounds() const;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class Object;

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
//////////////////////////////////////////////////////////////////////