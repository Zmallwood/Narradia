#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapTypeSelectionScene : public SceneBase {
      public:
        MapTypeSelectionScene();

      private:
        void UpdateDerived() override;
        void RenderDerived() override;

        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}