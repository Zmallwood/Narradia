#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class UserMapSelectionScene : public SceneBase
    /*////////////////////////////////////////*/
    {
      public:
        UserMapSelectionScene();
        void UpdateDerived() override;
        void RenderDerived() override;

      private:
        RenderId rendid_background_image_;
    };
}