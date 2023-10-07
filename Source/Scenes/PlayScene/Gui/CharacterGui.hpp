//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class CharacterGui : public GuiWindow, public Singleton<CharacterGui>
    /*/////////////////////////////////////////////////////////////////*/
    {
      public:
        CharacterGui();

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
        RenderId glIdExpText;
    };
}