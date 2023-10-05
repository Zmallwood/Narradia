//////////////////////////////////////////////////////////////////////
#pragma once
#include "PlaySceneGuiCore.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SkillsGui : public GuiWindow, public Singleton<SkillsGui>
    /*///////////////////////////////////////////////////////////*/ {
      public:
        SkillsGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
        std::vector<RenderId> glIdsSkillLabels;
        std::vector<RenderId> glIdsSkillLevelValues;
        std::vector<RenderId> glIdsSkillAddPointButton;
        RenderId glIdAvailablePoints;
    };
}
//////////////////////////////////////////////////////////////////////