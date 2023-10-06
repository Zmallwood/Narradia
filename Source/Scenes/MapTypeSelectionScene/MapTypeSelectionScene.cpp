//////////////////////////////////////////////////////////////////////
#include "MapTypeSelectionScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapTypeSelectionScene::Pimpl
    /*/////////////////////////////*/
    {
      public:
        const RectangleF rectBack = {0.0f, 0.0f, 1.0f, 1.0f};
        const RectangleF rectLogo = {0.4f, 0.1f, 0.2f, 0.05f};
        RenderId glIdBackImage;
        RenderId glIdLogoImage;
    };

    MapTypeSelectionScene::MapTypeSelectionScene()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////*/
    {
        CreateGui();
        auto sceneGui = GetSceneGui();
        sceneGui->AddGuiButton("Random wilderness map", {0.45f, 0.3f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeScene(SceneNames::MapGeneration);
        });
        sceneGui->AddGuiButton("User created map", {0.45f, 0.38f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeScene(SceneNames::UserMapSelection);
        });
        sceneGui->AddGuiButton("Back", {0.45f, 0.46f, 0.1f, 0.05f}, [=]() {
            SceneManager::Get().ChangeScene(SceneNames::MainMenu);
        });
        p->glIdBackImage = Renderer2DImages::Get().NewImage();
        p->glIdLogoImage = Renderer2DImages::Get().NewImage();
    }

    void MapTypeSelectionScene::UpdateDerived()
    /*//////////////////////////////////////*/
    {
        Log();
    }

    void MapTypeSelectionScene::RenderDerived()
    /*//////////////////////////////////////*/
    {
        Log();
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", p->glIdBackImage, p->rectBack);
        Renderer2DImages::Get().DrawImage("NarradiaLogo", p->glIdLogoImage, p->rectLogo);
    }
}
//////////////////////////////////////////////////////////////////////