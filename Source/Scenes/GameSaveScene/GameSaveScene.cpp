#include "GameSaveScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    GameSaveScene::GameSaveScene()
    /*//////////////////////////*/
    {
        CreateGui();
        rendid_background_image_ = Renderer2DImages::Get()->NewImage();
    }

    void
    GameSaveScene::UpdateDerived()
    /*//////////////////////////*/
    {
    }

    void
    GameSaveScene::RenderDerived()
    /*//////////////////////////*/
    {
        auto rect_back = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get()->DrawImage(
            "DefaultSceneBackground", rendid_background_image_, rect_back);
    }
}