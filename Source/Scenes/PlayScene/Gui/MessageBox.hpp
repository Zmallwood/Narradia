#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MessageBox : public GuiWindow {
      public:
        MessageBox(std::string_view, std::string_view);

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;

        std::string_view message_text_;
        RenderId rendid_message_text_;
    };
}