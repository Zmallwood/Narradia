//////////////////////////////////////////////////////////////////////
#include "TextOutBox.hpp"
#include "Graphics/Rendering/Renderer2DImages.hpp"
#include "Graphics/Rendering/Text/TextRenderer.hpp"
#include "Input/KeyboardInput.hpp"
#include "SceneManager.hpp"
#include "Scenes/PlayScene/Gui/ExperienceBar.hpp"
#include "TextLine.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TextOutBox::Pimpl
    /*///////////////////*/
    {
      public:
        RenderId glIdImage = 0;
        RenderId idSplitLine;
        RenderId idCommandLineInputArrow;
        RenderId idInputText;
        std::vector<TextLine> textLines;
        std::vector<GLuint> glIdTextLines;
        const RectangleF bounds = {0.0f, 0.8f, 0.3f, 0.2f};
        const float textLineHeight = 0.02f;
        const float kSplitLineHeight = 0.001f;
        bool enabled = true;
        bool inputActive = false;
        int cursorPosition = 0;
        std::string inputText;
    };

    TextOutBox::TextOutBox()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        p->glIdImage = Renderer2DImages::Get().NewImage();
        p->idSplitLine = Renderer2DImages::Get().NewImage();
        p->idCommandLineInputArrow = Renderer2DImages::Get().NewImage();
        p->idInputText = TextRenderer::Get().NewString();
        auto maxNumLines = GetMaxNumLines();
        for (auto i = 0; i < maxNumLines; i++)
            p->glIdTextLines.push_back(TextRenderer::Get().NewString());
    }

    void
    TextOutBox::Update()
    /*/////////////////////*/
    {
        if (KeyboardInput::Get().KeyHasBeenFiredPickResult(SDLK_RETURN))
        /**************************************************************/
        {
            p->inputActive = !p->inputActive;
        }
        if (p->inputActive)
        /*****************/
        {
            auto newTextInput = KeyboardInput::Get().PickTextInput();
            p->inputText.insert(p->cursorPosition, newTextInput);
            p->cursorPosition += newTextInput.length();
        }
    }

    void
    TextOutBox::Render() const
    /*///////////////////////////*/
    {
        if (!p->enabled)
            return;
        auto usedBounds = p->bounds;
        if (SceneManager::Get().GetCurrentScene() == SceneNames::Play)
            usedBounds = usedBounds.Translate(0.0f, -ExperienceBar::kBarHeight);
        Renderer2DImages::Get().DrawImage("TextOutBoxBack", p->glIdImage, usedBounds);
        auto maxNumLines = GetMaxNumLines();
        for (auto i = 0; i < maxNumLines; i++)
        /************************************/
        {
            auto textLineIndex = static_cast<int>(p->textLines.size()) - maxNumLines + i;
            if (textLineIndex >= 0)
            /*********************/
            {
                auto linePosY = usedBounds.y + (i + 1) * p->textLineHeight;
                Point2F position = {usedBounds.x + 0.01f, linePosY};
                TextRenderer::Get().DrawString(
                    p->glIdTextLines.at(i), p->textLines.at(textLineIndex).text, position,
                    p->textLines.at(textLineIndex).color);
            }
        }
        auto rect = RectangleF{
            0.0f, usedBounds.y + usedBounds.height - 1.3f * p->textLineHeight, p->bounds.width,
            p->kSplitLineHeight};
        Renderer2DImages::Get().DrawImage("Wheat", p->idSplitLine, rect);
        if (p->inputActive)
        /*****************/
        {
            auto rectArrow = RectangleF{
                0.0f, usedBounds.y + usedBounds.height - 1.3f * p->textLineHeight,
                p->textLineHeight, p->textLineHeight};
            Renderer2DImages::Get().DrawImage(
                "CommandLineInputArrow", p->idCommandLineInputArrow, rectArrow);
            auto posText =
                rectArrow.GetPosition().Translate(p->textLineHeight, p->textLineHeight / 2);
            auto usedText = p->inputText;
            if (SDL_GetTicks() % 600 < 300)
                usedText.insert(p->cursorPosition, "|");
            else
                usedText.insert(p->cursorPosition, " ");
            TextRenderer::Get().DrawString(p->idInputText, usedText, posText);
        }
    }

    void
    TextOutBox::Print(std::string_view text, Color textColor)
    /*//////////////////////////////////////////////////////////*/
    {
        if (!p->enabled)
            return;
        std::string printedText = std::string(GetCurrentTime().data()) + "." +
                                  std::to_string(SDL_GetTicks() % 1000) + ") " + text.data();
        p->textLines.push_back({printedText, textColor});
    }

    int
    TextOutBox::GetMaxNumLines() const
    /*//////////////////////////////////*/
    {
        return static_cast<int>(p->bounds.height / p->textLineHeight) - 2;
    }
}
//////////////////////////////////////////////////////////////////////