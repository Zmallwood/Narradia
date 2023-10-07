#include "GuiWindowCloseButton.hpp"
#include "Engine/Core/Cursor.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "GuiWindow.hpp"
#include "SceneGui.hpp"
#include "Scenes/EditorScene/EditorScene.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
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
        p->rendIdCloseButtonImg = Renderer2DImages::Get()->NewImage();
        p->parentWindow = parentWindow_;
    }

    void
    GuiWindowCloseButton::Update()
    /*//////////////////////////*/
    {
        p->hovered = false;
        if (p->GetBounds().Contains(GetMousePositionF()))
        /***********************************************/
        {
            Cursor::Get()->SetCursorType(CursorTypes::Hovering);
            p->hovered = true;
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "GuiWindowClose" + std::string(p->parentWindow->GetTitle()) +
                    std::to_string(SDL_GetTicks()),
                [&]
                /**********************/
                {
                    if (p->parentWindow->DestroyOnClose())
                    /************************************/
                    {
                        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Play)
                        /***************************************************/
                        {
                            auto sceneGui = PlayScene::Get()->GetSceneGui();
                            sceneGui->RemoveGuiComponent(p->parentWindow);
                        }
                        else if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
                        /************************************************************/
                        {
                            auto sceneGui = EditorScene::Get()->GetSceneGui();
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

    void
    GuiWindowCloseButton::Render() const
    /*////////////////////////////////*/
    {
        if (p->hovered)
            Renderer2DImages::Get()->DrawImage(
                "GuiWindowCloseButtonHovered", p->rendIdCloseButtonImg, p->GetBounds());
        else
            Renderer2DImages::Get()->DrawImage(
                "GuiWindowCloseButton", p->rendIdCloseButtonImg, p->GetBounds());
    }

    RectangleF
    GuiWindowCloseButton::Pimpl::GetBounds() const
    /*//////////////////////////////////////////*/
    {
        auto width = Pimpl::kWidth;
        auto height = ConvertWidthToHeight(width);
        return RectangleF{
            parentWindow->GetBounds().x + parentWindow->GetBounds().width - width - 0.005f,
            parentWindow->GetBounds().y + 0.03f / 2 - height / 2, width, height};
    }
}