//////////////////////////////////////////////////////////////////////
#pragma once
#include "Core/GuiCore.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class StatusPanel : public GuiComponent, public Singleton<StatusPanel>
    /*//////////////////////////////////////////////////////////////////*/ {
      public:
        StatusPanel();

      private:
        void Update() override;
        void Render() const override;
        const int hashPanelBg = Hash("PanelBg");
        const RectangleF boundsPanel = {0.0f, 0.0f, 0.2f, 0.11f};
        const RectangleF hpBarBounds = {0.08f, 0.03f, 0.1f, 0.01f};
        const RectangleF stamBarBounds = {0.08f, 0.06f, 0.1f, 0.01f};
        const std::string_view lblTextHealth = "Health";
        const std::string_view lblTextStamina = "Stamina";
        RenderId rendIdBgImage;
        RenderId rendIdPlHpText;
        RenderId rendIdPlStamText;
        RenderId rendIdPlHpBar;
        RenderId rendIdPlHpBarFilled;
        RenderId rendIdPlStamBar;
        RenderId rendIdPlStamBarFilled;
    };
}
//////////////////////////////////////////////////////////////////////