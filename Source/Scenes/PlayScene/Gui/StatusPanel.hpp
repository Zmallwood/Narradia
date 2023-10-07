#pragma once
#include "Engine/GuiCore/GuiComponent.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class StatusPanel : public GuiComponent, public Singleton<StatusPanel>
    /*//////////////////////////////////////////////////////////////////*/
    {
      public:
        StatusPanel();

      private:
        void Update() override;
        void Render() const override;

        const int kHashPanelBackground = Hash("PanelBg");
        const RectangleF kBoundsPanel = {0.0f, 0.0f, 0.2f, 0.11f};
        const RectangleF kHpBarBounds = {0.08f, 0.03f, 0.1f, 0.01f};
        const RectangleF kStaminaBarBounds = {0.08f, 0.06f, 0.1f, 0.01f};
        const RectangleF kRageBarBounds = {0.08f, 0.09f, 0.1f, 0.01f};
        const std::string_view kLabelTextHealth = "Health";
        const std::string_view kLabelTextStamina = "Stamina";
        const std::string_view kLabelTextRage = "Rage";
        RenderId renid_background_image_;
        RenderId rendid_health_text_;
        RenderId rendid_stamina_text_;
        RenderId rendid_rage_text_;
        RenderId rendid_health_bar_;
        RenderId rendid_health_bar_filled_;
        RenderId rendid_stamina_bar_;
        RenderId rendid_stamina_bar_filled_;
        RenderId rendid_rage_bar_;
        RenderId rendid_rage_bar_filled_;
    };
}