//////////////////////////////////////////////////////////////////////
#include "GuiButton.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "GuiContainer.hpp"
#include "Engine/Core/Cursor.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiButton::Pimpl
    /*//////////////////*/
    {
      public:
        std::string_view text;
        RectangleF bounds;
        std::function<void()> action;
        std::string_view imageName;
        std::string_view imageNameHovered;
        RenderId rendIdImage;
        RenderId rendIdLabelText;
        bool hovered;
        GuiContainer *parentContainer = nullptr;
    };

    GuiButton::GuiButton(
        const std::string_view &text_, RectangleF bounds_, std::function<void()> action_,
        GuiContainer *parentContainer_, const std::string_view &imageName_,
        const std::string_view &imageNameHovered_)
        : p(std::make_shared<Pimpl>())
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
        p->text = text_;
        p->bounds = bounds_;
        p->action = action_;
        p->parentContainer = parentContainer_;
        p->imageName = imageName_;
        p->imageNameHovered = imageNameHovered_;
        p->rendIdImage = Renderer2DImages::Get().NewImage();
        p->rendIdLabelText = TextRenderer::Get().NewString();
    }

    void GuiButton::Update()
    /*////////////////////*/
    {
        p->hovered = false;
        auto usedBounds = p->bounds;
        if (p->parentContainer)
        /*********************/
        {
            usedBounds.x += p->parentContainer->GetPosition().x;
            usedBounds.y += p->parentContainer->GetPosition().y;
        }
        if (usedBounds.Contains(GetMousePositionF()))
        /*******************************************/
        {
            Cursor::Get().SetCursorType(CursorTypes::Hovering);
            p->hovered = true;
            MouseInput::Get().GetLeftButton().AddFiredAction(
                "PerformGuiButtonAction",
                [&]
                /************************/
                { p->action(); },
                1);
        }
    }

    void GuiButton::Render() const
    /*//////////////////////////*/
    {
        const std::string_view *usedImageName;
        if (p->hovered)
            usedImageName = &p->imageNameHovered;
        else
            usedImageName = &p->imageName;
        auto usedBounds = p->bounds;
        if (p->parentContainer)
        /*********************/
        {
            usedBounds.x += p->parentContainer->GetPosition().x;
            usedBounds.y += p->parentContainer->GetPosition().y;
        }
        Renderer2DImages::Get().DrawImage(*usedImageName, p->rendIdImage, usedBounds);
        TextRenderer::Get().DrawString(
            p->rendIdLabelText, p->text, usedBounds.Center(), Colors::wheat, true);
    }
}
//////////////////////////////////////////////////////////////////////