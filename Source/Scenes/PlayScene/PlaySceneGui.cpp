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
    PlaySceneGui::PlaySceneGui() {
        Log();
        auto rect_gui_button_sys = RectangleF{0.95f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_repeat = RectangleF{0.91f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_window_skills =
            RectangleF{0.87f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_windows_inventory =
            RectangleF{0.83f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_window_character =
            RectangleF{0.79f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_window_build_selector =
            RectangleF{0.75f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto fn_sys = [] {
            PlaySceneGuiMenu::Get()->visible_ = !PlaySceneGuiMenu::Get()->visible_;
        };
        auto fnRepeat = [] { ActionRepeat::Get()->RepeatLastAction(); };
        auto fn_window_skills = [] { SkillsGui::Get()->ToggleVisibility(); };
        auto fn_window_inventory = [] { InventoryGui::Get()->ToggleVisibility(); };
        auto fn_window_character = [] { CharacterGui::Get()->ToggleVisibility(); };
        auto fn_window_build_selector = [] { BuildSelectorGui::Get()->ToggleVisibility(); };
        Log();
        AddGuiButton("", rect_gui_button_sys, fn_sys, "GuiButtonSys", "GuiButtonSysHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_repeat, fnRepeat, "GuiButtonRepeat", "GuiButtonRepeatHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_window_skills, fn_window_skills, "GuiButtonSkills",
            "GuiButtonSkillsHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_windows_inventory, fn_window_inventory, "GuiButtonInv",
            "GuiButtonInvHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_window_character, fn_window_character, "GuiButtonChar",
            "GuiButtonCharHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_window_build_selector, fn_window_build_selector,
            "GuiButtonBuildSelector", "GuiButtonBuildSelectorHovered");
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