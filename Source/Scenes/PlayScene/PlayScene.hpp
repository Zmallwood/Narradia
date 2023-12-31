#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class PlayScene : public SceneBase, public Singleton<PlayScene> {
      public:
        PlayScene();

      private:
        void Enter() override;
        void UpdateDerived() override;
        void RenderDerived() override;
        void RenderAfterGuiDerived() override;
    };
}