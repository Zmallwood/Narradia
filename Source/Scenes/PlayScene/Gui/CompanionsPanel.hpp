#pragma once
#include "Engine/GuiCore/GuiMovableContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class CompanionsPanel : public GuiMovableContainer, public Singleton<CompanionsPanel>
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
      public:
        CompanionsPanel();

      private:
        void Update() override;
        void Render() const override;
        static constexpr Point2F k_start_position = {0.02f, 0.2f};
        static constexpr SizeF k_size = {0.15f, 0.3f};
        static constexpr std::string_view k_title_text = "Companions";
        const RectangleF rel_bounds_horizontal_splitter;
        RenderId id_background_image;
        RenderId id_horizntal_splitter;
        RenderId id_title_text;
        std::array<RenderId, 4> ids_companions_label_text;
        std::array<RenderId, 4> ids_companions_stamina_back;
        std::array<RenderId, 4> ids_companions_stamina_filled;
    };
}