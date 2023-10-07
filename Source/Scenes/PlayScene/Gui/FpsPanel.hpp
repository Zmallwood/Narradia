#pragma once
#include "Engine/GuiCore/GuiMovableContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class FpsPanel : public GuiMovableContainer,
                     public Singleton<FpsPanel>
    /*////////////////////////////////////////*/
    {
      public:
        FpsPanel();

      private:
        void Update() override;
        void Render() const override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}