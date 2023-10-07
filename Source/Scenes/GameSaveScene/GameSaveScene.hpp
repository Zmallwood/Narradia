#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameSaveScene : public SceneBase
    /*////////////////////////////////*/
    {
      public:
        GameSaveScene();
        void UpdateDerived() override;
        void RenderDerived() override;

      private:
        RenderId glIdBackgroundImage;
    };
}