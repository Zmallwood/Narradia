#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameLoadScene : public SceneBase {
      public:
        GameLoadScene();
        void UpdateDerived() override;
        void RenderDerived() override;
    };
}