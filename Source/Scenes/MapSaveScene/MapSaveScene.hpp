//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapSaveScene : public SceneBase
    /*///////////////////////////////*/
    {
      public:
        MapSaveScene();
        void UpdateDerived() override;
        void RenderDerived() override;

      private:
        RenderId glIdBackgroundImage;
    };
}