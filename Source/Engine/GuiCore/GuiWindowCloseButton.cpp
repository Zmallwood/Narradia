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
        GuiWindow *parent_window_;
        bool hovered_ = false;
        RenderId rendid_close_button_image_;
    };

    GuiWindowCloseButton::GuiWindowCloseButton(GuiWindow *parentWindow_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////////*/
    {
        p->rendid_close_button_image_ = Renderer2DImages::Get()->NewImage();
        p->parent_window_ = parentWindow_;
    }

    void
    GuiWindowCloseButton::Update()
    /*//////////////////////////*/
    {
        p->hovered_ = false;
        if (p->GetBounds().Contains(GetMousePositionF()))
        /***********************************************/
        {
            Cursor::Get()->SetCursorType(CursorTypes::Hovering);
            p->hovered_ = true;
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "GuiWindowClose" + std::string(p->parent_window_->GetTitle()) +
                    std::to_string(SDL_GetTicks()),
                [&]
                /**********************/
                {
                    if (p->parent_window_->DestroyOnClose())
                    /************************************/
                    {
                        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Play)
                        /***************************************************/
                        {
                            auto scene_gui = PlayScene::Get()->GetSceneGui();
                            scene_gui->RemoveGuiComponent(p->parent_window_);
                        }
                        else if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
                        /************************************************************/
                        {
                            auto scene_gui = EditorScene::Get()->GetSceneGui();
                            scene_gui->RemoveGuiComponent(p->parent_window_);
                        }
                    }
                    else
                    /**/
                    {
                        p->parent_window_->Hide();
                    }
                },
                SDL_GetTicks() + 100);
        }
    }

    void
    GuiWindowCloseButton::Render() const
    /*////////////////////////////////*/
    {
        if (p->hovered_)
            Renderer2DImages::Get()->DrawImage(
                "GuiWindowCloseButtonHovered", p->rendid_close_button_image_, p->GetBounds());
        else
            Renderer2DImages::Get()->DrawImage(
                "GuiWindowCloseButton", p->rendid_close_button_image_, p->GetBounds());
    }

    RectangleF
    GuiWindowCloseButton::Pimpl::GetBounds() const
    /*//////////////////////////////////////////*/
    {
        auto width = Pimpl::kWidth;
        auto height = ConvertWidthToHeight(width);
        return RectangleF{
            parent_window_->GetBounds().x + parent_window_->GetBounds().width - width - 0.005f,
            parent_window_->GetBounds().y + 0.03f / 2 - height / 2, width, height};
    }
}