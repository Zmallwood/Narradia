//////////////////////////////////////////////////////////////////////
#include "GuiCore.hpp"
#include "Scenes/EditorScene/EditorScene.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "World/Object.hpp"
#include "Core/SceneManager.hpp"
#include "Core/Cursor.hpp"
#include "Core/Input/KeyboardInput.hpp"
#include "Core/Input/MouseInput.hpp"
#include "Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Core/Graphics/Rendering/Text/TextRenderer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiContainer::Pimpl
    /*/////////////////////*/
    {
      public:
        Point2F position;
        SizeF size;
        std::vector<std::shared_ptr<GuiButton>> guiButtons;
        std::vector<std::shared_ptr<GuiComponent>> guiComponents;
    };

    GuiContainer::GuiContainer(Point2F position_, SizeF size_)
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////////////////*/
    {
        p->position = position_;
        p->size = size_;
    }

    RectangleF GuiContainer::GetBounds() const
    /*//////////////////////////////////////*/
    {
        return {p->position.x, p->position.y, p->size.width, p->size.height};
    }

    const SizeF &GuiContainer::GetSize() const
    /*//////////////////////////////////////*/
    {
        return p->size;
    }

    const Point2F &GuiContainer::GetPosition() const
    /*////////////////////////////////////////////*/
    {
        return p->position;
    }

    void GuiContainer::SetPosition(Point2F newPosition)
    /*///////////////////////////////////////////////*/
    {
        p->position = newPosition;
    }

    void GuiContainer::Update()
    /*///////////////////////*/
    {
        for (auto &guiButton : p->guiButtons)
            guiButton->Update();
        for (auto &guiComponent : p->guiComponents)
            guiComponent->Update();
    }

    void GuiContainer::Render() const
    /*/////////////////////////////*/
    {
        for (auto &guiButton : p->guiButtons)
            guiButton->Render();
        for (auto &guiComponent : p->guiComponents)
            guiComponent->Render();
    }

    void GuiContainer::AddGuiButton(
        const std::string_view &text, RectangleF bounds, std::function<void()> action,
        const std::string_view &imageName, const std::string_view &imageNameHovered)
    /*////////////////////////////////////////////////////////////////////////////*/
    {
        std::shared_ptr<GuiButton> newGuiButton;
        if (imageName != "" && imageNameHovered != "")
            newGuiButton = std::make_shared<GuiButton>(
                text, bounds, action, this, imageName, imageNameHovered);
        else
            newGuiButton = std::make_shared<GuiButton>(text, bounds, action, this);
        p->guiButtons.push_back(newGuiButton);
    }

    void GuiContainer::AddGuiComponent(std::shared_ptr<GuiComponent> newComponent)
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        p->guiComponents.push_back(newComponent);
    }

    class GuiMovableContainer::Pimpl
    /*////////////////////////////*/
    {
      public:
        std::shared_ptr<Pimpl> p;
        RectangleF relDragableArea;
        bool movementTrigger = false;
        bool isMoving = false;
        Point2F mousePosStartMoving;
        Point2F posStartMoving;
    };

    GuiMovableContainer::GuiMovableContainer(
        RectangleF relDragableArea_, Point2F position_, SizeF size_)
        : GuiContainer(position_, size_),
          p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////*/
    {
        p->relDragableArea = relDragableArea_;
    }

    void GuiMovableContainer::Update()
    /*//////////////////////////////*/
    {
        auto mousePosF = GetMousePositionF();
        auto absDragableArea = p->relDragableArea.Translate(GetPosition().x, GetPosition().y);
        if (absDragableArea.Contains(mousePosF))
        /**************************************/
        {
            if (!p->isMoving && !p->movementTrigger)
            /**************************************/
            {
                MouseInput::Get().GetLeftButton().AddFiredAction(
                    "GuiWindowStartMoving" + std::to_string(SDL_GetTicks()),
                    [&]
                    /*****************/ {
                        p->posStartMoving = GetPosition();
                        p->mousePosStartMoving = GetMousePositionF();
                        p->isMoving = true;
                    },
                    SDL_GetTicks());
            }
        }
        if (p->isMoving)
        /**************/
        {
            auto dx = mousePosF.x - p->mousePosStartMoving.x;
            auto dy = mousePosF.y - p->mousePosStartMoving.y;
            SetPosition({p->posStartMoving.x + dx, p->posStartMoving.y + dy});
            p->movementTrigger = true;
            MouseInput::Get().GetLeftButton().AddReleasedAction(
                "GuiWindowStopMoving" + std::to_string(SDL_GetTicks()),
                [&]
                /*****************/
                {
                    p->isMoving = false;
                    p->movementTrigger = false;
                },
                SDL_GetTicks());
        }
        GuiContainer::Update();
    }

    void GuiMovableContainer::Render() const
    /*////////////////////////////////////*/
    {
        GuiContainer::Render();
    }

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

    void GuiTextBox::Update()
    /*/////////////////////*/
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

    void GuiTextBox::Render() const
    /*///////////////////////////*/
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

    std::string_view GuiTextBox::GetText()
    /*//////////////////////////////////*/
    {
        return p->text;
    }

    void GuiTextBox::SetText(const std::string &newText)
    /*////////////////////////////////////////////////*/
    {
        p->text = newText;
    }

    class SceneGui::Pimpl
    /*////////////////*/
    {
      public:
        std::vector<std::shared_ptr<GuiComponent>> guiComponents;
        std::vector<std::shared_ptr<GuiButton>> guiButtons;
    };

    SceneGui::SceneGui()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void SceneGui::AddGuiButton(
        std::string_view text, RectangleF bounds, std::function<void()> action,
        std::string_view imageName, std::string_view imageNameHovered)
    /*///////////////////////////////////////////////////////////////////////*/
    {
        std::shared_ptr<GuiButton> newGuiButton;
        if (imageName != "" && imageNameHovered != "")
            newGuiButton = std::make_shared<GuiButton>(
                text, bounds, action, nullptr, imageName, imageNameHovered);
        else
            newGuiButton = std::make_shared<GuiButton>(text, bounds, action);
        p->guiButtons.push_back(newGuiButton);
    }

    void SceneGui::Update()
    /*//////////////////*/
    {
        for (auto &guiButton : p->guiButtons)
            guiButton->Update();
        for (auto &guiComponent : p->guiComponents)
            guiComponent->Update();
    }

    void SceneGui::Render() const
    /*////////////////////////*/
    {
        Log();
        for (auto &guiButton : p->guiButtons | std::views::reverse)
        /*********************************************************/
        {
            guiButton->Render();
        }
        Log();
        for (auto &guiComponent : p->guiComponents | std::views::reverse)
        /***************************************************************/
        {
            guiComponent->Render();
        }
    }

    void SceneGui::AddGuiComponent(std::shared_ptr<GuiComponent> newComponent)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        p->guiComponents.push_back(newComponent);
    }

    void SceneGui::RemoveGuiComponent(GuiComponent *component)
    /*/////////////////////////////////////////////////////*/
    {
        auto i = 0;
        for (auto &entry : p->guiComponents)
        /**********************************/
        {
            if (entry.get() == component)
            /***************************/
            {
                p->guiComponents.erase(p->guiComponents.begin() + i);
                break;
            }
            i++;
        }
    }

    auto SceneGui::GetGuiComponents() const -> const std::vector<std::shared_ptr<GuiComponent>> &
    /*////////////////////////////////////////////////////////////////////////////////////////*/
    {
        return p->guiComponents;
    }

    class GuiWindowCloseButton::Pimpl
    /*/////////////////////////////*/
    {
      public:
        RectangleF GetBounds() const;
        static constexpr float kWidth = 0.01f;
        GuiWindow *parentWindow;
        bool hovered = false;
        RenderId rendIdCloseButtonImg;
    };

    GuiWindowCloseButton::GuiWindowCloseButton(GuiWindow *parentWindow_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////////*/
    {
        p->rendIdCloseButtonImg = Renderer2DImages::Get().NewImage();
        p->parentWindow = parentWindow_;
    }

    void GuiWindowCloseButton::Update()
    /*///////////////////////////////*/
    {
        p->hovered = false;
        if (p->GetBounds().Contains(GetMousePositionF()))
        /***********************************************/
        {
            Cursor::Get().SetCursorType(CursorTypes::Hovering);
            p->hovered = true;
            MouseInput::Get().GetLeftButton().AddFiredAction(
                "GuiWindowClose" + std::string(p->parentWindow->GetTitle()) +
                    std::to_string(SDL_GetTicks()),
                [&]
                /**********************/
                {
                    if (p->parentWindow->DestroyOnClose())
                    /************************************/
                    {
                        if (SceneManager::Get().GetCurrentScene() == SceneNames::Play)
                        /***************************************************/
                        {
                            auto sceneGui = PlayScene::Get().GetSceneGui();
                            sceneGui->RemoveGuiComponent(p->parentWindow);
                        }
                        else if (SceneManager::Get().GetCurrentScene() == SceneNames::Editor)
                        /************************************************************/
                        {
                            auto sceneGui = EditorScene::Get().GetSceneGui();
                            sceneGui->RemoveGuiComponent(p->parentWindow);
                        }
                    }
                    else
                    /**/
                    {
                        p->parentWindow->Hide();
                    }
                },
                SDL_GetTicks() + 100);
        }
    }

    void GuiWindowCloseButton::Render() const
    /*/////////////////////////////////////*/
    {
        if (p->hovered)
            Renderer2DImages::Get().DrawImage(
                "GuiWindowCloseButtonHovered", p->rendIdCloseButtonImg, p->GetBounds());
        else
            Renderer2DImages::Get().DrawImage(
                "GuiWindowCloseButton", p->rendIdCloseButtonImg, p->GetBounds());
    }

    RectangleF GuiWindowCloseButton::Pimpl::GetBounds() const
    /*/////////////////////////////////////////////////////*/
    {
        auto width = Pimpl::kWidth;
        auto height = ConvertWidthToHeight(width);
        return RectangleF{
            parentWindow->GetBounds().x + parentWindow->GetBounds().width - width - 0.005f,
            parentWindow->GetBounds().y + 0.03f / 2 - height / 2, width, height};
    }

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

    void GuiWindow::ToggleVisibility()
    /*//////////////////////////////*/
    {
        p->visible = !p->visible;
    }

    std::string_view GuiWindow::GetTitle()
    /*//////////////////////////////////*/
    {
        return p->title;
    }

    bool GuiWindow::DestroyOnClose()
    /*////////////////////////////*/
    {
        return p->destroyOnClose;
    }

    void GuiWindow::Show()
    /*//////////////////*/
    {
        p->visible = true;
    }

    void GuiWindow::Hide()
    /*//////////////////*/
    {
        p->visible = false;
    }

    float GuiWindow::GetMargin()
    /*////////////////////////*/
    {
        return Pimpl::kMargin;
    }

    float GuiWindow::GetTitleBarHeight()
    /*////////////////////////////////*/
    {
        return Pimpl::kTitleBarHeight;
    }

    void GuiWindow::Update()
    /*////////////////////*/
    {
        if (!p->visible)
            return;
        p->closeButton.Update();
        UpdateDerived();
        GuiMovableContainer::Update();
    }

    void GuiWindow::Render() const
    /*//////////////////////////*/
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

    RectangleF GuiWindow::GetAbsoluteTitleBarBounds() const
    /*///////////////////////////////////////////////////*/
    {
        return {GetPosition().x, GetPosition().y, GetSize().width, Pimpl::kTitleBarHeight};
    }

    GuiWindowObjectSlot::GuiWindowObjectSlot(
        GuiWindow *parentWindow_, int x_, int y_, int i_, float slotWidth_,
        const std::map<int, std::shared_ptr<Object>> &objectsList_)
        : parentWindow(parentWindow_),
          x(x_),
          y(y_),
          i(i_),
          slotWidth(slotWidth_),
          objectsList(objectsList_)
    /*///////////////////////////////////////////////////////////////////*/
    {
        rendIdFrame = Renderer2DImages::Get().NewImage();
        rendIdObject = Renderer2DImages::Get().NewImage();
        rendIdQtyText = TextRenderer::Get().NewString();
        rendIdTransformProgressBack = Renderer2DImages::Get().NewImage();
        rendIdTransformProgressFilled = Renderer2DImages::Get().NewImage();
    }

    void GuiWindowObjectSlot::Update(int offset) const
    /*//////////////////////////////////////////////*/
    {
        auto mousePosF = GetMousePositionF();
        if (GetBounds().Contains(mousePosF))
        /**********************************/
        {
            hoveredIndex = i + offset;
            if (objectsList.count(i + offset) > 0)
            /************************************/
            {
                auto object = objectsList.at(i + offset);
                hoveredObject = object.get();
                return;
            }
        }
    }

    void GuiWindowObjectSlot::Render(int offset) const
    /*//////////////////////////////////////////////*/
    {
        Log();
        auto slotHeight = GetSlotHeight();
        Log();
        Renderer2DImages::Get().DrawImage(frameImgNameHash, rendIdFrame, GetBounds());
        Log();
        Object *object = nullptr;
        Log();
        if (objectsList.count(i + offset) > 0)
        /************************************/
        {
            Log();
            object = objectsList.at(i + offset).get();
        }
        Log();
        if (object)
        /*********/
        {
            Log();
            auto imageNameHash = object->GetObjectType();
            Renderer2DImages::Get().DrawImage(imageNameHash, rendIdObject, GetBounds());
            if (object->GetQuantity() > 1)
            /****************************/
            {
                Log();
                auto qtyText = "x" + std::to_string(object->GetQuantity());
                TextRenderer::Get().DrawString(
                    rendIdQtyText, qtyText,
                    GetBounds().GetPosition().Translate(slotWidth * 0.65f, slotHeight * 0.7f));
            }
            if (object->GetTransformationProgress() > 0.0f)
            /*********************************************/
            {
                Log();
                auto pos =
                    GetBounds().GetPosition().Translate(slotWidth * 0.01f, slotHeight * 0.1f);
                auto height = slotHeight * 0.2f;
                auto width = slotWidth * 0.8f;
                auto widthFilled = width * object->GetTransformationProgress();
                auto rect = RectangleF{pos.x, pos.y, width, height};
                auto rectFilled = RectangleF{pos.x, pos.y, widthFilled, height};
                Renderer2DImages::Get().DrawImage("Black", rendIdTransformProgressBack, rect);
                Renderer2DImages::Get().DrawImage(
                    "Yellow", rendIdTransformProgressFilled, rectFilled);
            }
        }
    }

    RectangleF GuiWindowObjectSlot::GetBounds() const
    /*/////////////////////////////////////////////*/
    {
        auto bounds = parentWindow->GetBounds();
        auto margin = parentWindow->GetMargin();
        auto slotHeight = GetSlotHeight();
        auto titleBarHeight = parentWindow->GetTitleBarHeight();
        float xPos = bounds.x + margin + x * (slotWidth + margin);
        float yPos = bounds.y + titleBarHeight + margin + y * (slotHeight + margin);
        float w = slotWidth;
        float h = slotHeight;
        return {xPos, yPos, w, h};
    }

    float GuiWindowObjectSlot::GetSlotHeight() const
    /*////////////////////////////////////////////*/
    {
        return ConvertWidthToHeight(slotWidth);
    }
}
//////////////////////////////////////////////////////////////////////