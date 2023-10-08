#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct Point2F {
        Point2F Copy();
        Point2F Translate(float, float) const;
        Point2 ToIntPoint() const;
        float x = 0.0f;
        float y = 0.0f;
    };
}