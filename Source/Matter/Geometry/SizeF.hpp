#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct SizeF {
        RectangleF ToRectangle() const;
        float width = 0.0f;
        float height = 0.0f;
    };
}