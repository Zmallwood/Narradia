//////////////////////////////////////////////////////////////////////
#include "SkillsGui.hpp"
#include "Engine/Core/Cursor.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    SkillsGui::SkillsGui()
        : GuiWindow("Skills", {0.55f, 0.2f, 0.17f, 0.35f})
    /*//////////////////////////////////////////////////*/
    {
        for (auto i = 0; i < Player::Get()->skillSet.skills.size(); i++)
        /***********************************************************/
        {
            glIdsSkillLabels.push_back(TextRenderer::Get()->NewString());
            glIdsSkillLevelValues.push_back(TextRenderer::Get()->NewString());
            glIdsSkillAddPointButton.push_back(Renderer2DImages::Get()->NewImage());
        }
        glIdAvailablePoints = TextRenderer::Get()->NewString();
    }

    void
    SkillsGui::UpdateDerived()
    /*//////////////////////*/
    {
        auto mousePos = GetMousePositionF();
        auto i = 0;
        for (auto &skill : Player::Get()->skillSet.skills)
        /*********************************************/
        {
            if (Player::Get()->skillSet.availablePoints > 0)
            /*******************************************/
            {
                RectangleF addPointButtonRect = {
                    GetPosition().x + 0.15f, GetPosition().y + 0.04f + i * 0.03f, 0.01f,
                    ConvertWidthToHeight(0.01f)};
                if (addPointButtonRect.Contains(mousePos))
                /****************************************/
                {
                    Cursor::Get()->SetCursorType(CursorTypes::Hovering);
                    MouseInput::Get()->GetLeftButton().AddFiredAction(
                        "AddPointToSkill", [&]
                        /********************/
                        {
                            skill.second.level += 0.1f;
                            Player::Get()->skillSet.availablePoints--;
                        });
                }
            }
            i++;
        }
    }

    void
    SkillsGui::RenderDerived() const
    /*////////////////////////////*/
    {
        auto mousePos = GetMousePositionF();
        auto i = 0;
        for (auto &skill : Player::Get()->skillSet.skills)
        /*********************************************/
        {
            TextRenderer::Get()->DrawString(
                glIdsSkillLabels.at(i), skill.second.label.data(),
                GetPosition().Translate(0.01f, 0.05f + i * 0.03f));
            std::stringstream stream;
            stream << std::fixed << std::setprecision(1) << skill.second.level;
            std::string_view s = stream.str();
            TextRenderer::Get()->DrawString(
                glIdsSkillLevelValues.at(i), s, GetPosition().Translate(0.13f, 0.05f + i * 0.03f));
            if (Player::Get()->skillSet.availablePoints > 0)
            /*******************************************/
            {
                RectangleF addPointButtonRect = {
                    GetPosition().x + 0.15f, GetPosition().y + 0.04f + i * 0.03f, 0.01f,
                    ConvertWidthToHeight(0.01f)};
                if (addPointButtonRect.Contains(mousePos))
                /****************************************/
                {
                    Renderer2DImages::Get()->DrawImage(
                        "SkillsWindowAddPointButtonHovered", glIdsSkillAddPointButton.at(i),
                        addPointButtonRect);
                }
                else
                /**/
                {
                    Renderer2DImages::Get()->DrawImage(
                        "SkillsWindowAddPointButton", glIdsSkillAddPointButton.at(i),
                        addPointButtonRect);
                }
            }
            i++;
        }
        std::string_view availPointsText =
            "Available points: " + std::to_string(Player::Get()->skillSet.availablePoints);
        TextRenderer::Get()->DrawString(
            glIdAvailablePoints, availPointsText,
            GetPosition().Translate(0.01f, 0.07f + i * 0.03f));
    }
}