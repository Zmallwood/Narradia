#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SizeF
    /*///////*/
    {
      public:
        RectangleF ToRectangle() const;
        float width = 0.0f;
        float height = 0.0f;
    };
}