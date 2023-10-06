//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/GuiCore.hpp"
#include "Engine/GuiCore/GuiWindow.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class FpsPanel : public GuiMovableContainer, public Singleton<FpsPanel>
    /*///////////////////////////////////////////////////////////////////*/
    {
      public:
        FpsPanel();

      private:
        void Update() override;
        void Render() const override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
    class Object;
    class MessageBox : public GuiWindow
    /*///////////////////////////////*/
    {
      public:
        MessageBox(std::string_view, std::string_view);

      private:
        void UpdateDerived() override;
        void RenderDerived() const override;
        std::string_view messageText;
        RenderId glIdMessageText;
    };
    class ExperienceBar : public GuiComponent, public Singleton<ExperienceBar>
    /*//////////////////////////////////////////////////////////////////////*/
    {
      public:
        ExperienceBar();
        static constexpr float kBarHeight = 0.025f;

      private:
        void Update() override;
        void Render() const override;
        RenderId glIdUnfilledArea;
        RenderId glIdFilledArea;
    };
}
//////////////////////////////////////////////////////////////////////