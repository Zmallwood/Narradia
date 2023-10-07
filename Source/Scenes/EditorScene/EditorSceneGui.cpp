#include "EditorSceneGui.hpp"
#include "Gui/EditorSceneGuiMenu.hpp"
#include "Gui/GroundBrowserGui.hpp"
#include "Gui/MobBrowserGui.hpp"
#include "Gui/ObjectBrowserGui.hpp"
#include "Gui/RoofBrowserGui.hpp"
#include "Scenes/EditorScene/Modules/ToolUsing.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    EditorSceneGui::EditorSceneGui() {
        auto rect_gui_button_sys = RectangleF{0.95f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_mob_browser =
            RectangleF{0.91f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_roof_browser =
            RectangleF{0.87f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_objects_browser =
            RectangleF{0.83f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_ground_browser =
            RectangleF{0.79f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_raise_ground =
            RectangleF{0.75f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_lower_ground =
            RectangleF{0.71f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_expand = RectangleF{0.67f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        auto rect_gui_button_contract = RectangleF{0.63f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto fn_sys = [] {
            EditorSceneGuiMenu::Get()->visible = !EditorSceneGuiMenu::Get()->visible;
        };
        Log();
        auto fn_mob_browser = [] { MobBrowserGui::Get()->ToggleVisibility(); };
        Log();
        auto fn_roof_browser = [] { RoofBrowserGui::Get()->ToggleVisibility(); };
        Log();
        auto fn_objects_browser = [] { ObjectBrowserGui::Get()->ToggleVisibility(); };
        Log();
        auto fn_ground_browser = [] { GroundBrowserGui::Get()->ToggleVisibility(); };
        Log();
        auto fn_raise_ground = [] {
            ToolUsing::Get()->IncreaseElevationChange();
            ToolUsing::Get()->ChangeTool(Tools::AlterElevation);
        };
        Log();
        auto fn_lower_ground = [] {
            ToolUsing::Get()->DecreaseElevationChange();
            ToolUsing::Get()->ChangeTool(Tools::AlterElevation);
        };
        Log();
        auto fn_expand = [] { ToolUsing::Get()->IncreaseToolRadius(); };
        Log();
        auto fn_contract = [] { ToolUsing::Get()->DecreaseToolRadius(); };
        Log();
        AddGuiButton("", rect_gui_button_sys, fn_sys, "GuiButtonSys", "GuiButtonSysHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_mob_browser, fn_mob_browser, "GuiButtonMobBrowser",
            "GuiButtonMobBrowserHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_roof_browser, fn_roof_browser, "GuiButtonRoofBrowser",
            "GuiButtonRoofBrowserHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_objects_browser, fn_objects_browser, "GuiButtonObjectsBrowser",
            "GuiButtonObjectsBrowserHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_ground_browser, fn_ground_browser, "GuiButtonGroundBrowser",
            "GuiButtonGroundBrowserHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_raise_ground, fn_raise_ground, "GuiButtonRaiseGround",
            "GuiButtonRaiseGroundHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_lower_ground, fn_lower_ground, "GuiButtonLowerGround",
            "GuiButtonLowerGroundHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_expand, fn_expand, "GuiButtonExpand", "GuiButtonExpandHovered");
        Log();
        AddGuiButton(
            "", rect_gui_button_contract, fn_contract, "GuiButtonContract",
            "GuiButtonContractHovered");
        Log();
        AddGuiComponent(std::shared_ptr<EditorSceneGuiMenu>(EditorSceneGuiMenu::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<ObjectBrowserGui>(ObjectBrowserGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<GroundBrowserGui>(GroundBrowserGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<RoofBrowserGui>(RoofBrowserGui::Get()));
        Log();
        AddGuiComponent(std::shared_ptr<MobBrowserGui>(MobBrowserGui::Get()));
    }
}