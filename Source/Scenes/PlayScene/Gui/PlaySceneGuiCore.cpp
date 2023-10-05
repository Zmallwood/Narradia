//////////////////////////////////////////////////////////////////////
#include "PlaySceneGuiCore.hpp"
#include "Core/Rendering.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class FpsPanel::Pimpl
    /*/////////////////*/ {
      public:
        static constexpr Point2F kStartPosition = {0.9f, 0.01f};
        static constexpr SizeF kSize = {0.08f, 0.04f};
        int fps = 0;
        int framesCount = 0;
        int ticksLastUpdate = 0;
        RenderId idBackgroundImage;
        RenderId idFpsText;
        bool visible = false;
    }; // Class

    FpsPanel::FpsPanel()
        : p(std::make_shared<Pimpl>()),
          GuiMovableContainer(Pimpl::kSize.ToRectangle(), Pimpl::kStartPosition, Pimpl::kSize)
    /*//////////////////////////////////////////////////////////////////////////////////////*/ {
        p->idBackgroundImage = Renderer2DImages::Get().NewImage();
        p->idFpsText = TextRenderer::Get().NewString();
    } // Function

    void FpsPanel::Update()
    /*///////////////////*/ {
        if (SDL_GetTicks() > p->ticksLastUpdate + 1000)
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ {
            p->fps = p->framesCount++;
            p->framesCount = 0;
            p->ticksLastUpdate = SDL_GetTicks();
        }
        p->framesCount++;
        GuiMovableContainer::Update();
    } // Function

    void FpsPanel::Render() const
    /*/////////////////////////*/ {
        if (!p->visible) return;
        Renderer2DImages::Get().DrawImage(Hash("PanelBg"), p->idBackgroundImage, GetBounds());
        std::string_view fpsText = "Fps: " + std::to_string(p->fps);
        TextRenderer::Get().DrawString(
            p->idFpsText, fpsText, GetPosition().Translate(0.014f, 0.018f));
        GuiMovableContainer::Render();
    } // Function

    MessageBox::MessageBox(std::string_view title_, std::string_view messageText_)
        : GuiWindow(title_, {0.25f, 0.35f, 0.5f, 0.1f}, true, "PanelBgBlue"),
          messageText(messageText_)
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        Show();
        glIdMessageText = TextRenderer::Get().NewString();
        auto mainScene = PlayScene::GetPointer();
        auto sceneGui = mainScene->GetSceneGui();
        std::function<void()> confirmAction = [=, this] { sceneGui->RemoveGuiComponent(this); };
        AddGuiButton("Ok", {0.45f, 0.06f, 0.035f, ConvertWidthToHeight(0.015f)}, confirmAction);
    } // Function

    void MessageBox::UpdateDerived()
    /*////////////////////////////*/ {} // Function

    void MessageBox::RenderDerived() const
    /*//////////////////////////////////*/ {
        TextRenderer::Get().DrawString(
            glIdMessageText, messageText, GetPosition().Translate(0.01f, 0.05f), Colors::yellow);
    } // Function

    ExperienceBar::ExperienceBar()
    /*//////////////////////////*/ {
        glIdUnfilledArea = Renderer2DSolidColors::Get().NewRectangle();
        glIdFilledArea = Renderer2DImages::Get().NewImage();
    } // Function

    void ExperienceBar::Update()
    /*////////////////////////*/ {} // Function

    void ExperienceBar::Render() const
    /*//////////////////////////////*/ {
        auto unfilledArea = RectangleF{0.0f, 1.0f - kBarHeight, 1.0f, kBarHeight};
        Renderer2DSolidColors::Get().FillRectangle(glIdUnfilledArea, unfilledArea, Colors::black);
        auto expProgress = GetFractionalExpProgress(Player::Get().data.exp);
        auto filledArea = RectangleF{0.0f, 1.0f - kBarHeight, expProgress, kBarHeight};
        Renderer2DImages::Get().DrawImage("Green", glIdFilledArea, filledArea);
    } // Function
}
//////////////////////////////////////////////////////////////////////