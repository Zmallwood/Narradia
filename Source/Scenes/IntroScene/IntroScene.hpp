#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class IntroScene : public SceneBase {
      public:
        IntroScene();

      private:
        void UpdateDerived() override;
        void RenderDerived() override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}