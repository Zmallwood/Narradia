#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiComponent {
      public:
        virtual void Update() = 0;
        virtual void Render() const = 0;
    };
}