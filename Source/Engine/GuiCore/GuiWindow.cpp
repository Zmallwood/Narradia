//////////////////////////////////////////////////////////////////////
#include "GuiWindow.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "GuiWindowCloseButton.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiWindow::Pimpl
    /*//////////////////*/
    {
      public:
        static constexpr float kTitleBarHeight = 0.03f;
        static constexpr float kMargin = 0.005f;
        std::string_view title = "Unnamed window";
        bool visible = false;
        bool destroyOnClose = false;
        GuiWindowCloseButton closeButton;
        RenderId backRendId;
        RenderId titleBarRendId;
        RenderId titleTextId;
        std::string_view backgroundImageName;
    };

    GuiWindow::GuiWindow(
        std::string_view title_, RectangleF bounds_, bool destroyOnClose_,
        std::string_view backgroundImageName_)
        : p(std::make_shared<Pimpl>()),
          GuiMovableContainer(
              {0.0f, 0.0f, bounds_.width, Pimpl::kTitleBarHeight}, bounds_.GetPosition(),
              bounds_.GetSize())
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
        std::tie(p->backRendId, p->titleBarRendId) = Renderer2DImages::Get().NewImages<2>();
        p->titleTextId = TextRenderer::Get().NewString();
        p->closeButton = GuiWindowCloseButton(this);
        p->backgroundImageName = backgroundImageName_;
        p->title = title_;
        p->destroyOnClose = destroyOnClose_;
    }

    void
    GuiWindow::ToggleVisibility()
    /*/////////////////////////*/
    {
        p->visible = !p->visible;
    }

    std::string_view
    GuiWindow::GetTitle()
    /*/////////////////*/
    {
        return p->title;
    }

    bool
    GuiWindow::DestroyOnClose()
    /*///////////////////////*/
    {
        return p->destroyOnClose;
    }

    void
    GuiWindow::Show()
    /*/////////////*/
    {
        p->visible = true;
    }

    void
    GuiWindow::Hide()
    /*/////////////*/
    {
        p->visible = false;
    }

    float
    GuiWindow::GetMargin()
    /*//////////////////*/
    {
        return Pimpl::kMargin;
    }

    float
    GuiWindow::GetTitleBarHeight()
    /*//////////////////////////*/
    {
        return Pimpl::kTitleBarHeight;
    }

    void
    GuiWindow::Update()
    /*///////////////*/
    {
        if (!p->visible)
            return;
        p->closeButton.Update();
        UpdateDerived();
        GuiMovableContainer::Update();
    }

    void
    GuiWindow::Render() const
    /*/////////////////////*/
    {
        Log();
        if (!p->visible)
            return;
        Log();
        Renderer2DImages::Get().DrawImage(p->backgroundImageName, p->backRendId, GetBounds());
        Log();
        Renderer2DImages::Get().DrawImage(
            "GuiWindowTitleBar", p->titleBarRendId, GetAbsoluteTitleBarBounds());
        p->closeButton.Render();
        Log();
        TextRenderer::Get().DrawString(
            p->titleTextId, p->title, GetBounds().GetPosition().Translate(0.005f, 0.01f));
        Log();
        RenderDerived();
        GuiMovableContainer::Render();
    }

    RectangleF
    GuiWindow::GetAbsoluteTitleBarBounds() const
    /*////////////////////////////////////////*/
    {
        return {GetPosition().x, GetPosition().y, GetSize().width, Pimpl::kTitleBarHeight};
    }
}
//////////////////////////////////////////////////////////////////////