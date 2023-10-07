#pragma once
#include "Engine/GuiCore/GuiContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class PlaySceneGuiMenu : public GuiContainer, public Singleton<PlaySceneGuiMenu>
    /*//////////////////////////////////////////////////////////////////////////*/
    {
      public:
        PlaySceneGuiMenu();
        void Update() override;
        void Render() const override;
        bool visible = false;

      private:
        RenderId glIdCanvasCover;
    };
}