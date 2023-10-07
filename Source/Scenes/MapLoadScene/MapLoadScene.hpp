//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapLoadScene : public SceneBase
    /*///////////////////////////////*/
    {
      public:
        MapLoadScene();
        void UpdateDerived() override;
        void RenderDerived() override;

      private:
        RenderId glIdBackgroundImage;
    };
}