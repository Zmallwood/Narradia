#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SkillsGui : public GuiWindow, public Singleton<SkillsGui> {
      public:
        SkillsGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;

        std::vector<RenderId> rendids_skills_labels_;
        std::vector<RenderId> rendids_skill_levels_values_;
        std::vector<RenderId> rendids_skill_add_points_buttons_;
        RenderId rendid_available_points_;
    };
}