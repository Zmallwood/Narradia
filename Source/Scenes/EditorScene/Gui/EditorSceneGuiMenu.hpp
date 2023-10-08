#pragma once
#include "Engine/GuiCore/GuiContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class EditorSceneGuiMenu : public GuiContainer, public Singleton<EditorSceneGuiMenu> {
      public:
        EditorSceneGuiMenu();
        void Update() override;
        void Render() const override;
        bool visible = false;

      private:
        RenderId rendid_canvas_cover_;
    };
}