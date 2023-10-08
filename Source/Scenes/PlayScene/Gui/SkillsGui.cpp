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
        for (auto i = 0; i < Player::Get()->skill_set_.skills_.size(); i++)
        /***********************************************************/
        {
            rendids_skills_labels_.push_back(TextRenderer::Get()->NewString());
            rendids_skill_levels_values_.push_back(TextRenderer::Get()->NewString());
            rendids_skill_add_points_buttons_.push_back(Renderer2DImages::Get()->NewImage());
        }
        rendid_available_points_ = TextRenderer::Get()->NewString();
    }

    void
    SkillsGui::UpdateDerived()
    /*//////////////////////*/
    {
        auto mouse_position = GetMousePositionF();
        auto i = 0;
        for (auto &skill : Player::Get()->skill_set_.skills_)
        /*********************************************/
        {
            if (Player::Get()->skill_set_.available_points_ > 0)
            /*******************************************/
            {
                RectangleF add_point_button_rect = {
                    GetPosition().x + 0.15f, GetPosition().y + 0.04f + i * 0.03f, 0.01f,
                    ConvertWidthToHeight(0.01f)};
                if (add_point_button_rect.Contains(mouse_position))
                /****************************************/
                {
                    Cursor::Get()->SetCursorType(CursorTypes::Hovering);
                    MouseInput::Get()->GetLeftButton().AddFiredAction(
                        "AddPointToSkill", [&]
                        /********************/
                        {
                            skill.second.level += 0.1f;
                            Player::Get()->skill_set_.available_points_--;
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
        auto mouse_position = GetMousePositionF();
        auto i = 0;
        for (auto &skill : Player::Get()->skill_set_.skills_)
        /*********************************************/
        {
            TextRenderer::Get()->DrawString(
                rendids_skills_labels_.at(i), skill.second.label.data(),
                GetPosition().Translate(0.01f, 0.05f + i * 0.03f));
            std::stringstream stream;
            stream << std::fixed << std::setprecision(1) << skill.second.level;
            std::string_view s = stream.str();
            TextRenderer::Get()->DrawString(
                rendids_skill_levels_values_.at(i), s, GetPosition().Translate(0.13f, 0.05f + i * 0.03f));
            if (Player::Get()->skill_set_.available_points_ > 0)
            /*******************************************/
            {
                RectangleF add_point_button_rect = {
                    GetPosition().x + 0.15f, GetPosition().y + 0.04f + i * 0.03f, 0.01f,
                    ConvertWidthToHeight(0.01f)};
                if (add_point_button_rect.Contains(mouse_position))
                /****************************************/
                {
                    Renderer2DImages::Get()->DrawImage(
                        "SkillsWindowAddPointButtonHovered", rendids_skill_add_points_buttons_.at(i),
                        add_point_button_rect);
                }
                else
                /**/
                {
                    Renderer2DImages::Get()->DrawImage(
                        "SkillsWindowAddPointButton", rendids_skill_add_points_buttons_.at(i),
                        add_point_button_rect);
                }
            }
            i++;
        }
        std::string_view available_points_text =
            "Available points: " + std::to_string(Player::Get()->skill_set_.available_points_);
        TextRenderer::Get()->DrawString(
            rendid_available_points_, available_points_text,
            GetPosition().Translate(0.01f, 0.07f + i * 0.03f));
    }
}