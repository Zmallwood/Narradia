#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MainMenuScene : public SceneBase {
      public:
        MainMenuScene();

      private:
        void UpdateDerived() override;
        void RenderDerived() override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}