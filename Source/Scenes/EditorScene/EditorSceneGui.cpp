//////////////////////////////////////////////////////////////////////
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
        Log();
        auto rectGButtonSys = RectangleF{0.95f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonMobBrowser = RectangleF{0.91f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonRoofBrowser = RectangleF{0.87f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonObjectsBrowser =
            RectangleF{0.83f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonGroundBrowser = RectangleF{0.79f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonRaiseGround = RectangleF{0.75f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonLowerGround = RectangleF{0.71f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonExpand = RectangleF{0.67f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto rectGButtonContract = RectangleF{0.63f, 0.9f, 0.03f, ConvertWidthToHeight(0.03f)};
        Log();
        auto fnSys = [] { EditorSceneGuiMenu::Get().visible = !EditorSceneGuiMenu::Get().visible; };
        Log();
        auto fnMobBrowser = [] { MobBrowserGui::Get().ToggleVisibility(); };
        Log();
        auto fnRoofBrowser = [] { RoofBrowserGui::Get().ToggleVisibility(); };
        Log();
        auto fnObjectsBrowser = [] { ObjectBrowserGui::Get().ToggleVisibility(); };
        Log();
        auto fnGroundBrowser = [] { GroundBrowserGui::Get().ToggleVisibility(); };
        Log();
        auto fnRaiseGround = [] {
            ToolUsing::Get().IncreaseElevationChange();
            ToolUsing::Get().ChangeTool(Tools::AlterElevation);
        };
        Log();
        auto fnLowerGround = [] {
            ToolUsing::Get().DecreaseElevationChange();
            ToolUsing::Get().ChangeTool(Tools::AlterElevation);
        };
        Log();
        auto fnExpand = [] { ToolUsing::Get().IncreaseToolRadius(); };
        Log();
        auto fnContract = [] { ToolUsing::Get().DecreaseToolRadius(); };
        Log();
        AddGuiButton("", rectGButtonSys, fnSys, "GuiButtonSys", "GuiButtonSysHovered");
        Log();
        AddGuiButton(
            "", rectGButtonMobBrowser, fnMobBrowser, "GuiButtonMobBrowser",
            "GuiButtonMobBrowserHovered");
        Log();
        AddGuiButton(
            "", rectGButtonRoofBrowser, fnRoofBrowser, "GuiButtonRoofBrowser",
            "GuiButtonRoofBrowserHovered");
        Log();
        AddGuiButton(
            "", rectGButtonObjectsBrowser, fnObjectsBrowser, "GuiButtonObjectsBrowser",
            "GuiButtonObjectsBrowserHovered");
        Log();
        AddGuiButton(
            "", rectGButtonGroundBrowser, fnGroundBrowser, "GuiButtonGroundBrowser",
            "GuiButtonGroundBrowserHovered");
        Log();
        AddGuiButton(
            "", rectGButtonRaiseGround, fnRaiseGround, "GuiButtonRaiseGround",
            "GuiButtonRaiseGroundHovered");
        Log();
        AddGuiButton(
            "", rectGButtonLowerGround, fnLowerGround, "GuiButtonLowerGround",
            "GuiButtonLowerGroundHovered");
        Log();
        AddGuiButton("", rectGButtonExpand, fnExpand, "GuiButtonExpand", "GuiButtonExpandHovered");
        Log();
        AddGuiButton(
            "", rectGButtonContract, fnContract, "GuiButtonContract", "GuiButtonContractHovered");
        Log();
        AddGuiComponent(std::shared_ptr<EditorSceneGuiMenu>(EditorSceneGuiMenu::GetPointer()));
        Log();
        AddGuiComponent(std::shared_ptr<ObjectBrowserGui>(ObjectBrowserGui::GetPointer()));
        Log();
        AddGuiComponent(std::shared_ptr<GroundBrowserGui>(GroundBrowserGui::GetPointer()));
        Log();
        AddGuiComponent(std::shared_ptr<RoofBrowserGui>(RoofBrowserGui::GetPointer()));
        Log();
        AddGuiComponent(std::shared_ptr<MobBrowserGui>(MobBrowserGui::GetPointer()));
    }
}
//////////////////////////////////////////////////////////////////////