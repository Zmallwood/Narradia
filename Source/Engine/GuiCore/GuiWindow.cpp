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
        std::string_view title_ = "Unnamed window";
        bool visible_ = false;
        bool destroy_on_close_ = false;
        GuiWindowCloseButton gui_window_close_button_;
        RenderId rendid_background_;
        RenderId rendid_title_bar_;
        RenderId rendid_title_text_;
        std::string_view background_image_name_;
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
        std::tie(p->rendid_background_, p->rendid_title_bar_) = Renderer2DImages::Get()->NewImages<2>();
        p->rendid_title_text_ = TextRenderer::Get()->NewString();
        p->gui_window_close_button_ = GuiWindowCloseButton(this);
        p->background_image_name_ = backgroundImageName_;
        p->title_ = title_;
        p->destroy_on_close_ = destroyOnClose_;
    }

    void
    GuiWindow::ToggleVisibility()
    /*/////////////////////////*/
    {
        p->visible_ = !p->visible_;
    }

    std::string_view
    GuiWindow::GetTitle()
    /*/////////////////*/
    {
        return p->title_;
    }

    bool
    GuiWindow::DestroyOnClose()
    /*///////////////////////*/
    {
        return p->destroy_on_close_;
    }

    void
    GuiWindow::Show()
    /*/////////////*/
    {
        p->visible_ = true;
    }

    void
    GuiWindow::Hide()
    /*/////////////*/
    {
        p->visible_ = false;
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
        if (!p->visible_)
            return;
        p->gui_window_close_button_.Update();
        UpdateDerived();
        GuiMovableContainer::Update();
    }

    void
    GuiWindow::Render() const
    /*/////////////////////*/
    {
        Log();
        if (!p->visible_)
            return;
        Log();
        Renderer2DImages::Get()->DrawImage(p->background_image_name_, p->rendid_background_, GetBounds());
        Log();
        Renderer2DImages::Get()->DrawImage(
            "GuiWindowTitleBar", p->rendid_title_bar_, GetAbsoluteTitleBarBounds());
        p->gui_window_close_button_.Render();
        Log();
        TextRenderer::Get()->DrawString(
            p->rendid_title_text_, p->title_, GetBounds().GetPosition().Translate(0.005f, 0.01f));
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