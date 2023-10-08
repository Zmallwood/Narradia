#include "CharacterGui.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    CharacterGui::CharacterGui()
        : GuiWindow("Character", {0.15f, 0.2f, 0.17f, 0.35f})
    /*/////////////////////////////////////////////////////*/
    {
        rendid_exp_text_ = TextRenderer::Get()->NewString();
    }

    void
    CharacterGui::UpdateDerived()
    /*/////////////////////////*/
    {
    }

    void
    CharacterGui::RenderDerived() const
    /*///////////////////////////////*/
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2)
               << (100.0f * GetFractionalExpProgress(Player::Get()->data.experience_));
        std::string exp_progress = stream.str() + " %";
        TextRenderer::Get()->DrawString(
            rendid_exp_text_, "Experience: " + exp_progress,
            GetBounds().GetPosition().Translate(0.02f, 0.05f));
    }
}