//////////////////////////////////////////////////////////////////////
#include "BuildSelectorGui.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    BuildSelectorGui::BuildSelectorGui()
        : GuiWindow("Build selector", {0.25f, 0.2f, 0.17f, 0.35f})
    /*/////////////////////////////////////////////////////*/
    {
        AddGuiButton("None", {0.02f, 0.04f, 0.055f, ConvertWidthToHeight(0.015f)}, [] {
            Player::Get().playerBuild = PlayerBuilds::None;
        });
        AddGuiButton("Sword", {0.02f, 0.09f, 0.055f, ConvertWidthToHeight(0.015f)}, [] {
            Player::Get().playerBuild = PlayerBuilds::Sword;
        });
        AddGuiButton("Club", {0.02f, 0.14f, 0.055f, ConvertWidthToHeight(0.015f)}, [] {
            Player::Get().playerBuild = PlayerBuilds::Club;
        });
    }

    void
    BuildSelectorGui::UpdateDerived()
    /*//////////////////////////////*/
    {
    }

    void
    BuildSelectorGui::RenderDerived() const
    /*////////////////////////////////////*/
    {
    }
}
//////////////////////////////////////////////////////////////////////