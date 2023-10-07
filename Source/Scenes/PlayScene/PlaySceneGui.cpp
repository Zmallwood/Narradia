#include "PlaySceneGui.hpp"
#include "Gui/BuildSelectorGui.hpp"
#include "Gui/CharacterGui.hpp"
#include "Gui/CompanionsPanel.hpp"
#include "Gui/ExperienceBar.hpp"
#include "Gui/FpsPanel.hpp"
#include "Gui/InventoryGui.hpp"
#include "Gui/ObjectivesPanel.hpp"
#include "Gui/PlaySceneGuiMenu.hpp"
#include "Gui/SkillsGui.hpp"
#include "Gui/StatusPanel.hpp"
#include "Modules/ActionRepeat.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    PlaySceneGui::PlaySceneGui()
    /*////////////////////////*/
    {
        Log();
        auto rectGButtonSys = RectangleF{0.95f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonRepeat = RectangleF{0.91f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonWinSkills = RectangleF{0.87f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonWinInv = RectangleF{0.83f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonWinChar = RectangleF{0.79f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonWinBuildSelector =
            RectangleF{0.75f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto fnSys = [] { PlaySceneGuiMenu::Get()->visible_ = !PlaySceneGuiMenu::Get()->visible_; };
        Log();
        auto fnRepeat = [] { ActionRepeat::Get()->RepeatLastAction(); };
        Log();
        auto fnWinSkills = [] { SkillsGui::Get()->ToggleVisibility(); };
        Log();
        auto fnWinInv = [] { InventoryGui::Get()->ToggleVisibility(); };
        Log();
        auto fnWinChar = [] { CharacterGui::Get()->ToggleVisibility(); };
        Log();
        auto fnWinBuildSelector = [] { BuildSelectorGui::Get()->ToggleVisibility(); };
        Log();
        AddGuiButton("", rectGButtonSys, fnSys, "GuiButtonSys", "GuiButtonSysHovered");
        Log();
        AddGuiButton("", rectGButtonRepeat, fnRepeat, "GuiButtonRepeat", "GuiButtonRepeatHovered");
        Log();
        AddGuiButton(
            "", rectGButtonWinSkills, fnWinSkills, "GuiButtonSkills", "GuiButtonSkillsHovered");
        Log();
        AddGuiButton("", rectGButtonWinInv, fnWinInv, "GuiButtonInv", "GuiButtonInvHovered");
        Log();
        AddGuiButton("", rectGButtonWinChar, fnWinChar, "GuiButtonChar", "GuiButtonCharHovered");
        Log();
        AddGuiButton(
            "", rectGButtonWinBuildSelector, fnWinBuildSelector, "GuiButtonBuildSelector",
            "GuiButtonBuildSelectorHovered");
        Log();
        AddGuiComponent(std::shared_ptr<PlaySceneGuiMenu>(PlaySceneGuiMenu::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<StatusPanel>(StatusPanel::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<CharacterGui>(CharacterGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<InventoryGui>(InventoryGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<SkillsGui>(SkillsGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<BuildSelectorGui>(BuildSelectorGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<FpsPanel>(FpsPanel::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<ObjectivesPanel>(ObjectivesPanel::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<CompanionsPanel>(CompanionsPanel::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<ExperienceBar>(ExperienceBar::Get()));
    }
}