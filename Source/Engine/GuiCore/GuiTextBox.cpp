//////////////////////////////////////////////////////////////////////
#include "GuiTextBox.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Input/KeyboardInput.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "GuiContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiTextBox::Pimpl
    /*///////////////////*/
    {
      public:
        inline static GuiTextBox *activeTextBox = nullptr;
        std::shared_ptr<std::function<void()>> textChangeEvent;
        RenderId glIdBackgroundImage;
        RenderId glIdText;
        RectangleF bounds;
        GuiContainer *parentContainer = nullptr;
        int cursorPosition = 0;
        std::string text;
    };

    GuiTextBox::GuiTextBox(
        RectangleF bounds_, GuiContainer *parentContainer_, std::string text_,
        std::shared_ptr<std::function<void()>> textChangeEvent_)
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////////////////////////////////*/
    {
        p->bounds = bounds_;
        p->parentContainer = parentContainer_;
        p->textChangeEvent = textChangeEvent_;
        p->glIdBackgroundImage = Renderer2DImages::Get().NewImage();
        p->glIdText = TextRenderer::Get().NewString();
        p->cursorPosition = text_.length();
        p->text = text_;
    }

    void
    GuiTextBox::Update()
    /*////////////////*/
    {
        auto mousePosF = GetMousePositionF();
        auto usedBounds = p->bounds;
        if (p->parentContainer)
        /*********************/
        {
            usedBounds.x += p->parentContainer->GetPosition().x;
            usedBounds.y += p->parentContainer->GetPosition().y;
        }
        if (usedBounds.Contains(mousePosF))
        /*********************************/
        {
            MouseInput::Get().GetLeftButton().AddFiredAction(
                "ClickTextBox",
                [&]
                /********************/
                {
                    GuiTextBox::Pimpl::activeTextBox = this;
                    KeyboardInput::Get().ResetTextInput();
                },
                0);
        }
        if (Pimpl::activeTextBox == this)
        /*******************************/
        {
            auto newTextInput = KeyboardInput::Get().PickTextInput();
            p->text += newTextInput;
            p->cursorPosition += newTextInput.length();
            if (newTextInput != "")
                if (p->textChangeEvent)
                    (*p->textChangeEvent)();
            if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_RIGHT))
            /***********************************************************/
            {
                p->cursorPosition =
                    std::min(static_cast<int>(p->text.length()), p->cursorPosition + 1);
            }
            else if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_LEFT))
            /*****************************************************************/
            {
                p->cursorPosition = std::max(0, p->cursorPosition - 1);
            }
            else if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_BACKSPACE))
            /**********************************************************************/
            {
                if (p->cursorPosition > 0)
                /************************/
                {
                    p->text.erase(p->cursorPosition - 1, 1);
                    p->cursorPosition--;
                    if (p->textChangeEvent)
                        (*p->textChangeEvent)();
                }
            }
            else if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_DELETE))
            /*******************************************************************/
            {
                if (p->cursorPosition < p->text.length())
                /***************************************/
                {
                    p->text.erase(p->cursorPosition, 1);
                    if (p->textChangeEvent)
                        (*p->textChangeEvent)();
                }
            }
        }
    }

    void
    GuiTextBox::Render() const
    /*//////////////////////*/
    {
        auto usedBounds = p->bounds;
        if (p->parentContainer)
        /*********************/
        {
            usedBounds.x += p->parentContainer->GetPosition().x;
            usedBounds.y += p->parentContainer->GetPosition().y;
        }
        if (Pimpl::activeTextBox == this)
            Renderer2DImages::Get().DrawImage(
                "GTextBoxActiveBackground", p->glIdBackgroundImage, usedBounds);
        else
            Renderer2DImages::Get().DrawImage(
                "GTextBoxBackground", p->glIdBackgroundImage, usedBounds);
        auto usedText = p->text;
        if (Pimpl::activeTextBox == this)
        /*******************************/
        {
            if (SDL_GetTicks() % 1000 > 500)
                usedText.insert(p->cursorPosition, "|");
            else
                usedText.insert(p->cursorPosition, " ");
        }
        TextRenderer::Get().DrawString(
            p->glIdText, usedText, usedBounds.GetPosition().Translate(0.005f, 0.015f));
    }

    std::string_view
    GuiTextBox::GetText()
    /*/////////////////*/
    {
        return p->text;
    }

    void
    GuiTextBox::SetText(const std::string &newText)
    /*///////////////////////////////////////////*/
    {
        p->text = newText;
    }
}
//////////////////////////////////////////////////////////////////////