//////////////////////////////////////////////////////////////////////
#include "CharacterGui.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    CharacterGui::CharacterGui()
        : GuiWindow("Character", {0.15f, 0.2f, 0.17f, 0.35f})
    /*/////////////////////////////////////////////////////*/
    {
        glIdExpText = TextRenderer::Get().NewString();
    }

    void CharacterGui::UpdateDerived()
    /*//////////////////////////////*/
    {
    }

    void CharacterGui::RenderDerived() const
    /*////////////////////////////////////*/
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2)
               << (100.0f * GetFractionalExpProgress(Player::Get().data.exp));
        std::string expProgress = stream.str() + " %";
        TextRenderer::Get().DrawString(
            glIdExpText, "Experience: " + expProgress,
            GetBounds().GetPosition().Translate(0.02f, 0.05f));
    }
}
//////////////////////////////////////////////////////////////////////