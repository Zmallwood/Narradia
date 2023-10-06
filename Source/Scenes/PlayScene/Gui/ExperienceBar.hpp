//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ExperienceBar : public GuiComponent, public Singleton<ExperienceBar>
    /*//////////////////////////////////////////////////////////////////////*/
    {
      public:
        ExperienceBar();
        static constexpr float kBarHeight = 0.025f;

      private:
        void Update() override;
        void Render() const override;
        RenderId glIdUnfilledArea;
        RenderId glIdFilledArea;
    };
}
//////////////////////////////////////////////////////////////////////