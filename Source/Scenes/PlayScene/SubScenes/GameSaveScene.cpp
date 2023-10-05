//////////////////////////////////////////////////////////////////////
#include "GameSaveScene.hpp"
#include "Core/Rendering.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    GameSaveScene::GameSaveScene()
    /*////////////////////////*/
    {
        CreateGui();
        glIdBackgroundImage = Renderer2DImages::Get().NewImage();
    }

    void GameSaveScene::UpdateDerived()
    /*//////////////////////////////*/
    {
    }

    void GameSaveScene::RenderDerived()
    /*//////////////////////////////*/
    {
        auto rectBack = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", glIdBackgroundImage, rectBack);
    }
}
//////////////////////////////////////////////////////////////////////